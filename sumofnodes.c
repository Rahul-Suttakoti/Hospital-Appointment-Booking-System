#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int info;
    struct node *next;
} NODE;

NODE* frontins(NODE *, int);
void display(NODE *);
int sumList(NODE *);          // Sum of all nodes
int sumAlternate(NODE *, int); // Sum of alternate nodes

int main() {
    NODE *list = NULL;
    int choice, x, type;

    while (1) {
        printf("\nEnter 1->Front Insertion\n");
        printf("2->Display\n");
        printf("3->Sum of All Nodes\n");
        printf("4->Sum of Alternate Nodes\n");
        printf("5->Exit\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter the value to insert: ");
            scanf("%d", &x);
            list = frontins(list, x);
            break;

        case 2:
            display(list);
            break;

        case 3:
            printf("Sum of all nodes = %d\n", sumList(list));
            break;

        case 4:
            printf("Enter 1 for Odd-position sum, 2 for Even-position sum: ");
            scanf("%d", &type);
            printf("Sum = %d\n", sumAlternate(list, type));
            break;

        case 5:
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }
}

// Insert node at front
NODE* frontins(NODE *list, int x) {
    NODE *newnode;
    newnode = (NODE *)malloc(sizeof(NODE));
    newnode->info = x;
    newnode->next = list;
    return newnode;
}

// Display list
void display(NODE *list) {
    NODE *temp = list;
    if (list == NULL) {
        printf("Empty List..\n");
        return;
    }
    while (temp != NULL) {
        printf("\t%d", temp->info);
        temp = temp->next;
    }
    printf("\n");
}

// Sum of all nodes
int sumList(NODE *list) {
    int sum = 0;
    NODE *temp = list;

    while (temp != NULL) {
        sum += temp->info;
        temp = temp->next;
    }
    return sum;
}

// Sum of alternate nodes
int sumAlternate(NODE *list, int type) {
    int sum = 0, pos = 1;   // pos counts node positions
    NODE *temp = list;

    while (temp != NULL) {
        if ((type == 1 && pos % 2 != 0) || (type == 2 && pos % 2 == 0)) {
            sum += temp->info;
        }
        temp = temp->next;
        pos++;
    }
    return sum;
}
