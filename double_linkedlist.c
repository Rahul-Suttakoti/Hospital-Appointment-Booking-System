#include <stdio.h>
#include <stdlib.h>

typedef struct dnode {
    int info;
    struct dnode *left;
    struct dnode *right;
} DNODE;

/* prototypes */
DNODE* frontins(DNODE *dlist, int x);
DNODE* endins(DNODE *dlist, int x);
void display(DNODE *dlist);
DNODE* frontdel(DNODE *dlist);
DNODE* enddel(DNODE *dlist);
DNODE* succnodeins(DNODE *dlist, int x, int succ);

int main(void)
{
    DNODE *dlist = NULL;
    int choice, x, succ;

    for (;;) {
        printf("\nMenu:\n");
        printf("1 -> Front Insertion\n");
        printf("2 -> Display\n");
        printf("3 -> End Insertion\n");
        printf("4 -> Front deletion\n");
        printf("5 -> End deletion\n");
        printf("6 -> Insert after a node (give successor value)\n");
        printf("7 -> Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            /* clear bad input */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter the value to insert: ");
                if (scanf("%d", &x) == 1)
                    dlist = frontins(dlist, x);
                break;
            case 2:
                display(dlist);
                break;
            case 3:
                printf("Enter the value to insert: ");
                if (scanf("%d", &x) == 1)
                    dlist = endins(dlist, x);
                break;
            case 4:
                dlist = frontdel(dlist);
                break;
            case 5:
                dlist = enddel(dlist);
                break;
            case 6:
                printf("Enter the value to insert and the value after which to insert (succ): ");
                if (scanf("%d %d", &x, &succ) == 2)
                    dlist = succnodeins(dlist, x, succ);
                break;
            case 7:
                /* free entire list before exit */
                while (dlist != NULL) dlist = frontdel(dlist);
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

/* Insert at front */
DNODE* frontins(DNODE *dlist, int x)
{
    DNODE *newnode = malloc(sizeof(DNODE));
    if (!newnode) {
        perror("malloc");
        return dlist;
    }
    newnode->info = x;
    newnode->left = NULL;
    newnode->right = dlist;

    if (dlist != NULL)
        dlist->left = newnode;

    return newnode; /* new head */
}

/* Display list from head to tail */
void display(DNODE *dlist)
{
    DNODE *temp = dlist;
    if (dlist == NULL) {
        printf("Empty List..\n");
        return;
    }
    printf("List: ");
    while (temp != NULL) {
        printf("%d ", temp->info);
        temp = temp->right;
    }
    printf("\n");
}

/* Insert at end */
DNODE* endins(DNODE *dlist, int x)
{
    DNODE *newnode = malloc(sizeof(DNODE));
    if (!newnode) {
        perror("malloc");
        return dlist;
    }
    newnode->info = x;
    newnode->left = newnode->right = NULL;

    if (dlist == NULL) {
        return newnode;
    } else {
        DNODE *temp = dlist;
        while (temp->right != NULL)
            temp = temp->right;
        temp->right = newnode;
        newnode->left = temp;
        return dlist;
    }
}

/* Delete from front */
DNODE* frontdel(DNODE *dlist)
{
    if (dlist == NULL) {
        printf("Empty List..\n");
        return NULL;
    }

    DNODE *temp = dlist;
    if (dlist->right == NULL) {
        /* single node */
        free(temp);
        return NULL;
    } else {
        dlist = dlist->right;
        dlist->left = NULL;
        free(temp);
        return dlist;
    }
}

/* Delete from end */
DNODE* enddel(DNODE *dlist)
{
    if (dlist == NULL) {
        printf("Empty List..\n");
        return NULL;
    }

    DNODE *temp = dlist;
    /* if only one node */
    if (temp->right == NULL) {
        free(temp);
        return NULL;
    }

    while (temp->right != NULL)
        temp = temp->right; /* temp is last node */

    DNODE *prev = temp->left;
    prev->right = NULL;
    free(temp);
    return dlist;
}

/* Insert new node with value x after the first node which has value 'succ' */
/* If list is empty, new node becomes the list head. */
DNODE* succnodeins(DNODE *dlist, int x, int succ)
{
    DNODE *newnode = malloc(sizeof(DNODE));
    if (!newnode) {
        perror("malloc");
        return dlist;
    }
    newnode->info = x;
    newnode->left = newnode->right = NULL;

    if (dlist == NULL) {
        /* list empty: new node becomes head */
        return newnode;
    }

    DNODE *temp = dlist;
    /* find node with info == succ */
    while (temp != NULL && temp->info != succ)
        temp = temp->right;

    if (temp == NULL) {
        printf("No such node with value %d found.\n", succ);
        free(newnode);
        return dlist;
    }

    /* insert after temp */
    newnode->left = temp;
    newnode->right = temp->right;
    temp->right = newnode;
    if (newnode->right != NULL)
        newnode->right->left = newnode;

    return dlist;
}
