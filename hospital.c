/*
 hospital.c
 Simple Hospital Appointment Booking System (portable)

 Features:
 - Register patients
 - Create scheduled appointments (YYYY-MM-DD HH:MM) or walk-in
 - Walk-in check-in -> priority queue (min-heap) by (priority, arrival_time)
 - Assign next walk-in appointment to a doctor
 - Cancel appointment
 - List queued walk-ins and scheduled appointments
 - Persistence via CSV files: patients.csv, appointments.csv
 - NEW: Reset all data (clear patients & appointments and reset IDs)

 Compile:
   gcc -std=c11 -O2 hospital.c -o hospital

 Notes:
 - Uses sscanf for date parsing (portable across Windows/Linux).
 - Uses time_t stored as signed 64-bit when writing/reading CSV (cast via long long).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 128
#define INIT_CAP 64
#define PATIENTS_FILE "patients.csv"
#define APPTS_FILE "appointments.csv"

typedef enum { ST_QUEUED = 0, ST_SCHEDULED = 1, ST_IN_PROGRESS = 2, ST_COMPLETED = 3, ST_CANCELLED = 4 } Status;
const char *status_str[] = { "queued", "scheduled", "in_progress", "completed", "cancelled" };

typedef struct {
    int id;
    char name[MAX_NAME];
    char phone[32];
    char email[64];
} Patient;

typedef struct {
    int id;
    int patient_id;
    int doctor_id;           /* 0 = unassigned */
    time_t scheduled_at;     /* 0 if not scheduled (walk-in) */
    int priority;            /* 1 = emergency, 2 = urgent, 3 = routine */
    time_t arrival_time;
    Status status;
    char notes[256];
} Appointment;

/* Dynamic arrays */
static Patient *patients = NULL;
static int patients_n = 0, patients_cap = 0;

static Appointment *appts = NULL;
static int appts_n = 0, appts_cap = 0;

/* Heap storing indices into appts[] for queued (walk-in) appointments */
static int *heap = NULL;
static int heap_size = 0, heap_cap = 0;

/* --- Utilities --- */
static void *xrealloc(void *p, size_t newsize) {
    void *q = realloc(p, newsize);
    if (!q) { fprintf(stderr, "Out of memory\n"); exit(EXIT_FAILURE); }
    return q;
}

static void ensure_patients_cap() {
    if (patients_n >= patients_cap) {
        patients_cap = patients_cap ? patients_cap * 2 : INIT_CAP;
        patients = xrealloc(patients, patients_cap * sizeof(Patient));
    }
}
static void ensure_appts_cap() {
    if (appts_n >= appts_cap) {
        appts_cap = appts_cap ? appts_cap * 2 : INIT_CAP;
        appts = xrealloc(appts, appts_cap * sizeof(Appointment));
    }
}
static void ensure_heap_cap() {
    if (heap_size >= heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : INIT_CAP;
        heap = xrealloc(heap, heap_cap * sizeof(int));
    }
}

static int next_patient_id() { return patients_n ? (patients[patients_n-1].id + 1) : 1; }
static int next_appt_id()    { return appts_n ? (appts[appts_n-1].id + 1) : 1; }

/* Heap comparator: return non-zero if a has higher priority than b */
static int appt_higher_priority(int ia, int ib) {
    Appointment *a = &appts[ia];
    Appointment *b = &appts[ib];
    if (a->priority != b->priority) return a->priority < b->priority;
    if (a->arrival_time != b->arrival_time) return a->arrival_time < b->arrival_time;
    return a->id < b->id;
}

static void heap_swap_idx(int i, int j) {
    int tmp = heap[i]; heap[i] = heap[j]; heap[j] = tmp;
}
static void heap_push(int appt_idx) {
    ensure_heap_cap();
    heap[heap_size] = appt_idx;
    int i = heap_size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (appt_higher_priority(heap[i], heap[p])) { heap_swap_idx(i, p); i = p; }
        else break;
    }
}
static int heap_pop_index() {
    /* pop valid queued appt (skip stale entries) */
    while (heap_size > 0) {
        int top = heap[0];
        if (top < 0 || top >= appts_n || appts[top].status != ST_QUEUED) {
            /* remove top and reheapify */
            heap_swap_idx(0, heap_size - 1);
            heap_size--;
            int i = 0;
            for (;;) {
                int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
                if (l < heap_size && appt_higher_priority(heap[l], heap[smallest])) smallest = l;
                if (r < heap_size && appt_higher_priority(heap[r], heap[smallest])) smallest = r;
                if (smallest != i) { heap_swap_idx(i, smallest); i = smallest; } else break;
            }
            continue;
        }
        /* valid top */
        int res = top;
        heap_swap_idx(0, heap_size - 1);
        heap_size--;
        int i = 0;
        for (;;) {
            int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
            if (l < heap_size && appt_higher_priority(heap[l], heap[smallest])) smallest = l;
            if (r < heap_size && appt_higher_priority(heap[r], heap[smallest])) smallest = r;
            if (smallest != i) { heap_swap_idx(i, smallest); i = smallest; } else break;
        }
        return res;
    }
    return -1;
}

/* --- Persistence: CSV read/write --- */
/* We'll store time_t as integer using long long for portability in CSV */

static void save_patients(void) {
    FILE *f = fopen(PATIENTS_FILE, "w");
    if (!f) { perror("save_patients fopen"); return; }
    for (int i = 0; i < patients_n; ++i) {
        fprintf(f, "%d,%s,%s,%s\n",
                patients[i].id,
                patients[i].name[0] ? patients[i].name : "",
                patients[i].phone[0] ? patients[i].phone : "",
                patients[i].email[0] ? patients[i].email : "");
    }
    fclose(f);
}

static void save_appointments(void) {
    FILE *f = fopen(APPTS_FILE, "w");
    if (!f) { perror("save_appointments fopen"); return; }
    for (int i = 0; i < appts_n; ++i) {
        Appointment *a = &appts[i];
        /* remove newline chars from notes for CSV safety */
        char notes_clean[256] = "";
        int p = 0;
        for (int k = 0; a->notes[k] && p < (int)sizeof(notes_clean)-1; ++k) {
            if (a->notes[k] == '\n' || a->notes[k] == '\r') notes_clean[p++] = ' ';
            else notes_clean[p++] = a->notes[k];
        }
        notes_clean[p] = '\0';
        fprintf(f, "%d,%d,%d,%lld,%d,%lld,%d,%s\n",
                a->id,
                a->patient_id,
                a->doctor_id,
                (long long)a->scheduled_at,
                a->priority,
                (long long)a->arrival_time,
                (int)a->status,
                notes_clean);
    }
    fclose(f);
}

static void load_patients(void) {
    FILE *f = fopen(PATIENTS_FILE, "r");
    if (!f) return;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        int id = 0;
        char name[MAX_NAME] = "", phone[32] = "", email[64] = "";
        /* tolerant parsing */
        sscanf(line, "%d,%127[^,],%31[^,],%63[^\n]", &id, name, phone, email);
        if (id == 0) continue;
        ensure_patients_cap();
        patients[patients_n].id = id;
        strncpy(patients[patients_n].name, name, MAX_NAME-1);
        strncpy(patients[patients_n].phone, phone, sizeof(patients[patients_n].phone)-1);
        strncpy(patients[patients_n].email, email, sizeof(patients[patients_n].email)-1);
        patients_n++;
    }
    fclose(f);
}

static void load_appointments(void) {
    FILE *f = fopen(APPTS_FILE, "r");
    if (!f) return;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        Appointment a;
        memset(&a, 0, sizeof(a));
        long long sched=0, arr=0;
        int status_i = 0;
        char notes[256] = "";
        int n = sscanf(line, "%d,%d,%d,%lld,%d,%lld,%d,%255[^\n]",
                       &a.id, &a.patient_id, &a.doctor_id, &sched, &a.priority, &arr, &status_i, notes);
        if (n < 7) continue; /* malformed */
        a.scheduled_at = (time_t)sched;
        a.arrival_time = (time_t)arr;
        a.status = (Status)status_i;
        strncpy(a.notes, notes, sizeof(a.notes)-1);
        ensure_appts_cap();
        appts[appts_n++] = a;
    }
    fclose(f);
    /* rebuild heap for queued, unscheduled appts */
    for (int i = 0; i < appts_n; ++i) {
        if (appts[i].status == ST_QUEUED && appts[i].scheduled_at == 0) heap_push(i);
    }
}

/* --- Find helpers --- */
static Patient* find_patient(int pid) {
    for (int i = 0; i < patients_n; ++i) if (patients[i].id == pid) return &patients[i];
    return NULL;
}
static Appointment* find_appt_by_id(int aid) {
    for (int i = 0; i < appts_n; ++i) if (appts[i].id == aid) return &appts[i];
    return NULL;
}

/* --- Reset/Clear all data --- */
static void reset_all_data(void) {
    /* Free arrays and reset counters */
    if (patients) { free(patients); patients = NULL; }
    if (appts)    { free(appts);    appts = NULL; }
    if (heap)     { free(heap);     heap = NULL; }

    patients_n = 0; patients_cap = 0;
    appts_n = 0;    appts_cap = 0;
    heap_size = 0;  heap_cap = 0;

    /* Truncate CSV files by saving empty content */
    save_patients();
    save_appointments();

    printf("All patient and appointment data cleared. IDs reset. Files truncated.\n");
}

/* --- Core operations --- */
static void register_patient(void) {
    char name[MAX_NAME], phone[32], email[64];
    printf("Enter patient name: ");
    if (!fgets(name, sizeof(name), stdin)) return; name[strcspn(name, "\r\n")] = 0;
    printf("Phone: "); if (!fgets(phone, sizeof(phone), stdin)) return; phone[strcspn(phone, "\r\n")] = 0;
    printf("Email: "); if (!fgets(email, sizeof(email), stdin)) return; email[strcspn(email, "\r\n")] = 0;
    ensure_patients_cap();
    Patient p;
    memset(&p, 0, sizeof(p));
    p.id = next_patient_id();
    strncpy(p.name, name, sizeof(p.name)-1);
    strncpy(p.phone, phone, sizeof(p.phone)-1);
    strncpy(p.email, email, sizeof(p.email)-1);
    patients[patients_n++] = p;
    printf("Patient registered with id %d\n", p.id);
    save_patients();
}

static void create_scheduled_appointment(void) {
    int pid;
    char when_str[64] = "";
    int priority = 3;
    printf("Enter patient id: ");
    if (scanf("%d%*c", &pid) != 1) { printf("Invalid input\n"); return; }
    if (!find_patient(pid)) { printf("Patient not found\n"); return; }
    printf("Enter scheduled time (YYYY-MM-DD HH:MM) or leave blank for now: ");
    if (!fgets(when_str, sizeof(when_str), stdin)) when_str[0] = 0;
    when_str[strcspn(when_str, "\r\n")] = 0;
    printf("Priority (1=emergency,2=urgent,3=routine): ");
    if (scanf("%d%*c", &priority) != 1) { printf("Invalid input\n"); return; }

    time_t scheduled_at = 0;
    if (strlen(when_str) > 0) {
        int year, month, day, hour, min;
        if (sscanf(when_str, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &min) == 5) {
            struct tm tm = {0};
            tm.tm_year = year - 1900;
            tm.tm_mon  = month - 1;
            tm.tm_mday = day;
            tm.tm_hour = hour;
            tm.tm_min  = min;
            tm.tm_isdst = -1;
            scheduled_at = mktime(&tm);
            if (scheduled_at == (time_t)-1) scheduled_at = 0;
        } else {
            printf("Could not parse date/time — appointment will be created as queued (walk-in)\n");
        }
    }

    ensure_appts_cap();
    Appointment a;
    memset(&a,0,sizeof(a));
    a.id = next_appt_id();
    a.patient_id = pid;
    a.doctor_id = 0;
    a.scheduled_at = scheduled_at;
    a.priority = priority;
    a.arrival_time = scheduled_at ? scheduled_at : time(NULL);
    a.status = scheduled_at ? ST_SCHEDULED : ST_QUEUED;
    a.notes[0] = '\0';
    appts[appts_n++] = a;
    if (a.status == ST_QUEUED && a.scheduled_at == 0) heap_push(appts_n - 1);
    printf("Appointment created with id %d (status: %s)\n", a.id, status_str[a.status]);
    save_appointments();
}

static void checkin_walkin(void) {
    int pid, priority = 3;
    printf("Enter patient id: ");
    if (scanf("%d%*c", &pid) != 1) { printf("Invalid input\n"); return; }
    if (!find_patient(pid)) { printf("Patient not found\n"); return; }
    printf("Priority (1=emergency,2=urgent,3=routine): ");
    if (scanf("%d%*c", &priority) != 1) { printf("Invalid input\n"); return; }

    ensure_appts_cap();
    Appointment a;
    memset(&a, 0, sizeof(a));
    a.id = next_appt_id();
    a.patient_id = pid;
    a.doctor_id = 0;
    a.scheduled_at = 0;
    a.priority = priority;
    a.arrival_time = time(NULL);
    a.status = ST_QUEUED;
    a.notes[0] = '\0';
    appts[appts_n++] = a;
    heap_push(appts_n - 1);
    printf("Walk-in checked in with appointment id %d\n", a.id);
    save_appointments();
}

static void assign_next_to_doctor(void) {
    int doc_id;
    printf("Enter doctor id to assign to: ");
    if (scanf("%d%*c", &doc_id) != 1) { printf("Invalid input\n"); return; }
    int appt_idx = heap_pop_index();
    if (appt_idx < 0) { printf("No queued walk-in appointments\n"); return; }
    Appointment *a = &appts[appt_idx];
    a->doctor_id = doc_id;
    a->status = ST_IN_PROGRESS;
    printf("Assigned appointment id %d (patient %d) to doctor %d\n", a->id, a->patient_id, doc_id);
    save_appointments();
}

static void cancel_appointment_cli(void) {
    int aid;
    printf("Enter appointment id to cancel: ");
    if (scanf("%d%*c", &aid) != 1) { printf("Invalid input\n"); return; }
    Appointment *a = find_appt_by_id(aid);
    if (!a) { printf("Appointment not found\n"); return; }
    a->status = ST_CANCELLED;
    printf("Appointment %d cancelled\n", a->id);
    save_appointments();
}

static void list_queue(void) {
    printf("Queued walk-ins (priority -> arrival):\n");
    /* Gather indices, then simple sort by priority/arrival for display (stable for small sizes) */
    int *idx = malloc(sizeof(int) * (appts_n > 0 ? appts_n : 1));
    if (!idx) { printf("Memory error\n"); return; }
    int tn = 0;
    for (int i = 0; i < appts_n; ++i) if (appts[i].status == ST_QUEUED && appts[i].scheduled_at == 0) idx[tn++] = i;
    for (int i = 0; i < tn; ++i) {
        int best = i;
        for (int j = i + 1; j < tn; ++j)
            if (appt_higher_priority(idx[j], idx[best])) best = j;
        int t = idx[i]; idx[i] = idx[best]; idx[best] = t;
    }
    if (tn == 0) printf(" (empty)\n");
    for (int i = 0; i < tn; ++i) {
        Appointment *a = &appts[idx[i]];
        Patient *p = find_patient(a->patient_id);
        char atbuf[32] = "";
        struct tm *tm = localtime(&a->arrival_time);
        if (tm) strftime(atbuf, sizeof(atbuf), "%Y-%m-%d %H:%M", tm);
        printf("Appt id %d | Patient %s (id %d) | priority %d | arrival %s\n",
               a->id, p ? p->name : "Unknown", a->patient_id, a->priority, atbuf);
    }
    free(idx);
}

static void list_scheduled(void) {
    printf("Scheduled appointments:\n");
    int any = 0;
    for (int i = 0; i < appts_n; ++i) if (appts[i].status == ST_SCHEDULED) {
        Appointment *a = &appts[i];
        Patient *p = find_patient(a->patient_id);
        char sbuf[32] = "";
        struct tm *tm = localtime(&a->scheduled_at);
        if (tm) strftime(sbuf, sizeof(sbuf), "%Y-%m-%d %H:%M", tm);
        printf("Appt id %d | Patient %s (id %d) | scheduled at %s | priority %d\n",
               a->id, p ? p->name : "Unknown", a->patient_id, sbuf, a->priority);
        any = 1;
    }
    if (!any) printf(" (none)\n");
}

/* Menu */
static void print_menu(void) {
    puts("\n--- Hospital Appointment Booking ---");
    puts("1) Register patient");
    puts("2) Create scheduled appointment");
    puts("3) Walk-in check-in");
    puts("4) Assign next walk-in to doctor");
    puts("5) Cancel appointment");
    puts("6) List queue");
    puts("7) List scheduled");
    puts("8) Save and exit");
    puts("9) Reset all data (clear patients & appointments)");
    printf("Choose an option: ");
}

int main(void) {
    load_patients();
    load_appointments();
    printf("Loaded %d patients, %d appointments. Heap entries (approx) %d\n", patients_n, appts_n, heap_size);

    while (1) {
        print_menu();
        int opt;
        if (scanf("%d%*c", &opt) != 1) { puts("Invalid input"); break; }
        switch (opt) {
            case 1: register_patient(); break;
            case 2: create_scheduled_appointment(); break;
            case 3: checkin_walkin(); break;
            case 4: assign_next_to_doctor(); break;
            case 5: cancel_appointment_cli(); break;
            case 6: list_queue(); break;
            case 7: list_scheduled(); break;
            case 8:
                save_patients();
                save_appointments();
                puts("Saved. Exiting.");
                return 0;
            case 9:
                printf("Are you sure you want to RESET ALL DATA? This will permanently delete all patients and appointments. (Y/N): ");
                {
                    char confirm[8];
                    if (fgets(confirm, sizeof(confirm), stdin)) {
                        if (confirm[0] == 'y' || confirm[0] == 'Y') {
                            reset_all_data();
                        } else {
                            puts("Reset cancelled.");
                        }
                    } else {
                        puts("Reset cancelled.");
                    }
                }
                break;
            default: puts("Unknown option"); break;
        }
    }
    return 0;
}
