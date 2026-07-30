#include <stdio.h>
#include <stdlib.h>

void creategraph(void);
void display(void);
void bfs(int start);
void qinsert(int x);
int qdelete(void);
int isempty(void);


#define MAXV 10
#define QSIZE 25

int adj[MAXV][MAXV];
int visit[MAXV];
int q[QSIZE], f, r, n;

int main(void)
{
    int v, i;

    
    f = -1;
    r = -1;

    printf("Enter the number of vertices (max %d): ", MAXV-1);
    if (scanf("%d", &n) != 1 || n <= 0 || n >= MAXV) {
        fprintf(stderr, "Invalid number of vertices.\n");
        return 1;
    }

    for (i = 0; i < MAXV; ++i) {
        int j;
        visit[i] = 0;
        for (j = 0; j < MAXV; ++j) adj[i][j] = 0;
    }

    creategraph();
    printf("\nAdjacency matrix:\n");
    display();

    printf("\nEnter the start vertex (1..%d): ", n);
    if (scanf("%d", &v) != 1 || v < 1 || v > n) {
        fprintf(stderr, "Invalid start vertex.\n");
        return 1;
    }

    
    for (i = 1; i <= n; ++i) visit[i] = 0;

    printf("\nBFS order: ");
    bfs(v);
    printf("\n");

    return 0;
}

void creategraph(void)
{
    int i, j;
    while (1) {
        printf("Enter the Source and Destination Vertex (0 0 to stop): ");
        if (scanf("%d %d", &i, &j) != 2) {
            fprintf(stderr, "Invalid input. Stopping graph creation.\n");
            break;
        }
        if (i == 0 && j == 0) break;

        if (i < 1 || i > n || j < 1 || j > n) {
            printf("Vertices must be between 1 and %d. Try again.\n", n);
            continue;
        }

        adj[i][j] = 1;
    }
}

void display(void)
{
    int p, qx;
    for (p = 1; p <= n; p++) {
        for (qx = 1; qx <= n; qx++)
            printf("%d ", adj[p][qx]);
        printf("\n");
    }
}

void bfs(int v)
{
    int w;
    qinsert(v);
    visit[v] = 1;
    printf("%d ", v);

    while (!isempty()) {
        v = qdelete();
        if (v == -1) break; 
        for (w = 1; w <= n; w++) {
            if (visit[w] == 0 && adj[v][w] == 1) {
                qinsert(w);
                visit[w] = 1;
                printf("%d ", w);
            }
        }
    }
}

/* Queue insert (simple linear queue). Checks overflow. */
void qinsert(int x)
{
    if (r >= QSIZE - 1) {
        printf("Queue overflow. Cannot insert %d\n", x);
        return;
    }
    if (f == -1 && r == -1) {
        f = 0;
        r = -1;
    }
    r++;
    q[r] = x;
}

/* Queue delete: returns -1 on underflow. Advances front index. */
int qdelete(void)
{
    int x;
    if (isempty()) {
        return -1;
    }
    x = q[f];
    f++;
    /* if queue becomes empty, reset indices */
    if (f > r) {
        f = r = -1;
    }
    return x;
}

int isempty(void)
{
    if ((f == -1 && r == -1) || f > r)
        return 1;
    return 0;
}
