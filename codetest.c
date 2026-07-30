// singly linked circular list front insertion, end deletion and display


#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int info;
    struct node *next;
} NODE;

NODE* frontins(NODE *list, int x);
NODE* enddel(NODE *list);
void display(NODE *list);

int main() {
    NODE *list;
    int choice, x;

    list = NULL;

    while (1)
    {
        printf("\nEnter 1->Front Insertion\n2->Display\n");
        printf("3->End Deletion\n4->Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the value to insert: ");
            scanf("%d", &x);
            list = frontins(list, x);
            break;

        case 2:
            display(list);
            break;

        case 3:
            list = enddel(list);
            break;

        default:
            exit(0);
        }
    }
}

NODE* frontins(NODE *list, int x)
{
    NODE *newnode = (NODE *)malloc(sizeof(NODE));
    newnode->info = x;

    if (list == NULL) {
        newnode->next = newnode;   // circular
        list = newnode;
    }
    else {
        NODE *temp = list;
        while (temp->next != list)//goes to last node
            temp = temp->next;

        newnode->next = list;
        temp->next = newnode;
        list = newnode;
    }
    return list;
}

void display(NODE *list)
{
    if (list == NULL) {
        printf("Empty List..\n");
        return;
    }

    NODE *temp = list;
    do {
        printf("\t%d", temp->info);
        temp = temp->next;
    } while (temp != list);//till last node

    printf("\n");
}

NODE* enddel(NODE *list)
{
    if (list == NULL) {
        printf("Empty List..\n");
        return list;
    }
    if (list->next == list) {
        free(list);
        return NULL;
    }

    NODE *temp = list, *prev = NULL;
    while (temp->next != list) {
        prev = temp;
        temp = temp->next;
    }

    prev->next = list; 
    free(temp);

    return list;
}
