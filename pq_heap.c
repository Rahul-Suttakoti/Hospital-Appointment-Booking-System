#include <stdio.h>
#include <stdlib.h>

typedef struct pnode {
    int info;
    int pty;
} PNODE;

void pq_insert(int x, int p);
PNODE pq_del(void);
void adjust(void);
void display(void);

PNODE pq[25];
int count = 0;

int main(void)
{
    int choice, x, p;
    PNODE min;

    while (1)
    {
        printf("\nMenu:\n1 -> PQ_Insertion\n2 -> PQ_Deletion\n3 -> Display\n4 -> Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) { // guard against invalid input
            fprintf(stderr, "Invalid input. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        switch (choice)
        {
            case 1:
                printf("Enter the value and its priority: ");
                if (scanf("%d %d", &x, &p) != 2) {
                    fprintf(stderr, "Invalid input. Aborting insertion.\n");
                    // flush stdin could be added, but keep simple
                } else {
                    pq_insert(x, p);
                }
                break;

            case 2:
                if (count == 0) {
                    printf("Heap is empty. Nothing to delete.\n");
                } else {
                    min = pq_del();
                    printf("The deleted element is %d : %d\n", min.info, min.pty);
                }
                break;

            case 3:
                display();
                break;

            case 4:
                printf("Exiting.\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

/* Insert item (x) with priority p into min-heap */
void pq_insert(int x, int p)
{
    if (count >= (int)(sizeof(pq) / sizeof(pq[0]))) {
        printf("Priority queue full. Cannot insert.\n");
        return;
    }

    int i, j;
    PNODE key, temp;
    temp.info = x;
    temp.pty  = p;

    /* insert at the end */
    i = count;
    pq[i] = temp;
    key = pq[i];
    count++;

    /* bubble up (sift-up) to maintain min-heap on pty */
    while (i > 0) {
        j = (i - 1) / 2;
        if (pq[j].pty > key.pty) {
            pq[i] = pq[j];
            i = j;
        } else {
            break;
        }
    }
    pq[i] = key;
}

/* Delete and return the minimum element (root) from heap */
PNODE pq_del(void)
{
    PNODE min;
    if (count == 0) {
        /* return sentinel value if needed */
        min.info = -1;
        min.pty  = -1;
        return min;
    }

    min = pq[0];
    pq[0] = pq[count - 1];
    count--;
    if (count > 0)
        adjust();
    return min;
}

/* Sift-down (heapify) from root to restore min-heap property */
void adjust(void)
{
    int j = 0;
    int i = 2 * j + 1; /* left child index */
    PNODE key = pq[j];

    while (i < count) { /* while left child exists */
        /* if right child exists and is smaller, use right child */
        if (i + 1 < count && pq[i + 1].pty < pq[i].pty) {
            i = i + 1; /* i now index of smaller child */
        }

        /* if child is smaller than key, move child up */
        if (pq[i].pty < key.pty) {
            pq[j] = pq[i];
            j = i;
            i = 2 * j + 1;
        } else {
            break;
        }
    }
    pq[j] = key;
}

/* Display heap contents level-order (array order) */
void display(void)
{
    if (count == 0) {
        printf("Priority queue (heap) is empty.\n");
        return;
    }

    printf("Heap (index : value : priority)\n");
    for (int i = 0; i < count; ++i) {
        printf("[%d] : %d : %d\n", i, pq[i].info, pq[i].pty);
    }
}
