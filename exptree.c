#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
                    char info;
	                struct tree *left;
					struct tree *right;
                   }TREE;
				   
TREE* create_exptree(TREE *,char []);
void preorder(TREE *);
void postorder(TREE *);
void inorder(TREE*);
void push(TREE*);
TREE* pop();
int isoper(char);

TREE *stk[10];
int top;

void main()
 {
   TREE *root=NULL;
   char postfix[20];
	
	printf("Enter valid postfix expression");
	scanf("%s",postfix);
	
	root= create_exptree(root,postfix);
	printf("\nThe prefix expression is ..  ");
	preorder(root);
	printf("\nThe Infix expression is ..  ");
	inorder(root);
	printf("\nThe postfix expression is ..  ");
	postorder(root);
  }
  
 TREE* create_exptree(TREE *temp,char postfix[])
 {
	char ch;
	int i=0;
	
	ch=postfix[i];
	 while(ch!='\0')
	 {
     temp = (TREE *)(malloc(sizeof(TREE)));
	 temp->info=ch;
	 temp->left=NULL;
	 temp->right=NULL;
	 
	 if(isoper(ch))
	 {
		temp->right=pop();
		temp->left=pop();
		push(temp);
	 }
	 else push(temp);
		 i++;
		 ch=postfix[i];
	 }
	 temp=pop();
	 return temp;
 }
 
 void inorder(TREE *root)
{
	if(root)
	{
	  inorder(root->left);
	  printf("\t%c",root->info);
	  inorder(root->right);
	}
}

void preorder(TREE *root)
{
	if(root)
	{
	  printf("\t%c",root->info);
	  preorder(root->left);
	  preorder(root->right);
	}
}

void postorder(TREE *root)
{
	if(root)
	{
	  postorder(root->left);
	  postorder(root->right);
	  printf("\t%c",root->info);
	}
}
 
int isoper(char ch)
{
	switch(ch)
	{
	 case '+':
	 case '-':
	 case '*':
     case '/':
     case '^':return 1;
     default: return 0; 
	}
}

void push(TREE *t)
{   top++;
	stk[top]=t;
}

TREE* pop()
{ TREE *t;
  t=stk[top];
   top--;
  return t;
}