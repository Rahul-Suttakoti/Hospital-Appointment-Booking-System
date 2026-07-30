#include <stdio.h>
#include <stdlib.h>

// Singly Linked List
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* newNode(int data) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

Node* insertEnd(Node* head, int data) {
    Node* temp = newNode(data);
    if (!head) return temp;
    Node* p = head;
    while (p->next) p = p->next;
    p->next = temp;
    return head;
}

void display(Node* head) {
    while (head) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// Sum of all nodes
int sumAll(Node* head) {
    int sum = 0;
    while (head) {
        sum += head->data;
        head = head->next;
    }
    return sum;
}

// Sum of alternate nodes
int sumAlternate(Node* head) {
    int sum = 0, flag = 1;
    while (head) {
        if (flag) sum += head->data;
        flag = !flag;
        head = head->next;
    }
    return sum;
}

// Search and delete
Node* deleteNode(Node* head, int key) {
    if (!head) return NULL;
    if (head->data == key) {
        Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    Node* prev = head;
    Node* curr = head->next;
    while (curr) {
        if (curr->data == key) {
            prev->next = curr->next;
            free(curr);
            return head;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Element %d not found\n", key);
    return head;
}

// Doubly Linked List 
typedef struct DNode {
    int data;
    struct DNode *next, *prev;
} DNode;

DNode* newDNode(int data) {
    DNode* temp = (DNode*)malloc(sizeof(DNode));
    temp->data = data;
    temp->next = temp->prev = NULL;
    return temp;
}

DNode* insertDEnd(DNode* head, int data) {
    DNode* temp = newDNode(data);
    if (!head) return temp;
    DNode* p = head;
    while (p->next) p = p->next;
    p->next = temp;
    temp->prev = p;
    return head;
}

// Check palindrome using two-pointer technique
int isPalindrome(DNode* head) {
    if (!head) return 1;
    DNode* tail = head;
    while (tail->next) tail = tail->next;

    while (head != tail && head->prev != tail) {
        if (head->data != tail->data) return 0;
        head = head->next;
        tail = tail->prev;
    }
    return 1;
}

// Polynomial Linked List 
typedef struct PolyNode {
    int coeff, pow;
    struct PolyNode* next;
} PolyNode;

PolyNode* newPolyNode(int c, int p) {
    PolyNode* temp = (PolyNode*)malloc(sizeof(PolyNode));
    temp->coeff = c;
    temp->pow = p;
    temp->next = NULL;
    return temp;
}

PolyNode* insertPoly(PolyNode* head, int c, int p) {
    PolyNode* temp = newPolyNode(c, p);
    if (!head) return temp;
    PolyNode* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = temp;
    return head;
}

void displayPoly(PolyNode* head) {
    while (head) {
        printf("%dx^%d", head->coeff, head->pow);
        head = head->next;
        if (head) printf(" + ");
    }
    printf("\n");
}


int main() {
    Node* sll = NULL;      // singly linked list
    DNode* dll = NULL;     // doubly linked list
    PolyNode* poly = NULL; // polynomial list
    int choice, val, coeff, pow;

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Insert into Singly Linked List\n");
        printf("2. Display Singly Linked List\n");
        printf("3. Sum of all nodes (SLL)\n");
        printf("4. Sum of alternate nodes (SLL)\n");
        printf("5. Delete a node (SLL)\n");
        printf("6. Insert into Doubly Linked List\n");
        printf("7. Check Palindrome (DLL)\n");
        printf("8. Insert Polynomial Term\n");
        printf("9. Display Polynomial\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value: ");
            scanf("%d", &val);
            sll = insertEnd(sll, val);
            break;

        case 2:
            display(sll);
            break;

        case 3:
            printf("Sum of all nodes = %d\n", sumAll(sll));
            break;

        case 4:
            printf("Sum of alternate nodes = %d\n", sumAlternate(sll));
            break;

        case 5:
            printf("Enter value to delete: ");
            scanf("%d", &val);
            sll = deleteNode(sll, val);
            break;

        case 6:
            printf("Enter value: ");
            scanf("%d", &val);
            dll = insertDEnd(dll, val);
            break;

        case 7:
            if (isPalindrome(dll))
                printf("Doubly Linked List is a Palindrome\n");
            else
                printf("Not a Palindrome\n");
            break;

        case 8:
            printf("Enter coefficient and power: ");
            scanf("%d%d", &coeff, &pow);
            poly = insertPoly(poly, coeff, pow);
            break;

        case 9:
            displayPoly(poly);
            break;

        case 0:
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}













#include <stdio.h>
#include <stdlib.h>

// Binary Search Tree node 
typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

// Create a new node 
Node *createNode(int key) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}


Node *insert_unique(Node *root, int key) {
    if (root == NULL) {
        return createNode(key); //not found — create node 
    }

    if (key < root->key) {
        root->left = insert_unique(root->left, key);
    } else if (key > root->key) {
        root->right = insert_unique(root->right, key);
    } else {
        // duplicate key found; do nothing
        return root;
    }
    return root;
}

// Optional: iterative version that avoids recursion and clearly prevents duplicates 
Node *insert_unique_iter(Node *root, int key) {
    if (root == NULL) return createNode(key);

    Node *cur = root;
    Node *parent = NULL;

    while (cur != NULL) {
        parent = cur;
        if (key < cur->key) cur = cur->left;
        else if (key > cur->key) cur = cur->right;
        else {
            // duplicate key found; do nothing 
            return root;
        }
    }

    if (key < parent->key) parent->left = createNode(key);
    else parent->right = createNode(key);

    return root;
}

// In-order traversal (prints keys in sorted order) 
void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

//Free the tree 
void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}


int main(void) {
    Node *root = NULL;

    int items[] = {50, 30, 70, 20, 40, 60, 80, 30, 70};
    size_t n = sizeof(items)/sizeof(items[0]);

    for (size_t i = 0; i < n; ++i) {
        root = insert_unique(root, items[i]);
    }

    printf("In-order (unique keys): ");
    inorder(root);
    printf("\n");

    free_tree(root);
    return 0;
}
















#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

//Stack structure for holding tree nodes 
typedef struct Stack {
    Node **arr;
    int top;
    int capacity;
} Stack;

//Function to create a new tree node 
Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

//Stack utility functions 
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (Node**)malloc(capacity * sizeof(Node*));
    return stack;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, Node* node) {
    if (stack->top == stack->capacity - 1) {
        printf("Stack overflow!\n");
        return;
    }
    stack->arr[++stack->top] = node;
}

Node* pop(Stack* stack) {
    if (isEmpty(stack)) return NULL;
    return stack->arr[stack->top--];
}

//Iterative preorder traversal
void preorderIterative(Node* root) {
    if (root == NULL) return;

    Stack* stack = createStack(100); // assuming max 100 nodes
    push(stack, root);

    while (!isEmpty(stack)) {
        Node* curr = pop(stack);
        printf("%d ", curr->data);

        // Push right first so left is processed first
        if (curr->right) push(stack, curr->right);
        if (curr->left) push(stack, curr->left);
    }

    free(stack->arr);
    free(stack);
}


int main() {
    Node* root = createNode(10);
    root->left = createNode(5);
    root->right = createNode(20);
    root->left->left = createNode(3);
    root->left->right = createNode(7);
    root->right->left = createNode(15);
    root->right->right = createNode(30);

    printf("Preorder traversal (Iterative): ");
    preorderIterative(root);
    printf("\n");

    return 0;
}
