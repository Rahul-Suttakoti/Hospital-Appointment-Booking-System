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
