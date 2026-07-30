#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
                    int info;
	                struct tree *left;
					struct tree *right;
                   }TREE;
				   
TREE* insert(TREE *,int);
void iter_inorder(TREE*);
void push(TREE *);
TREE* pop();

TREE *stk[10];
int top;

void main()
 {
   TREE *root;
   int choice,x;
   top=-1;
   root=NULL;
   
   while(1)
   {
   printf("Enter 1->Insert\n2->INORDER\n3->EXIT\n");
   
   scanf("%d", &choice);
   switch(choice)
   {
   case 1:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          root=insert(root,x);
          }break;
   case 2:{
	      iter_inorder(root);
          }break;
   default: exit(0);
   }
  }
 }
 
TREE* insert(TREE *root,int x)
{ TREE *newnode,*temp;
  int flag=1;

	 newnode = (TREE *)(malloc(sizeof(TREE)));
	 newnode->info=x;
	 newnode->left=NULL;
	 newnode->right=NULL;
	 
	 if(root==NULL)root=newnode;
	 else { temp=root;
		    while(flag) // as long as node is not assigned
			{ 
		    if(x<temp->info)
			   if(temp->left==NULL){
				                   temp->left=newnode;
								   flag=0;
			                       }
			   else temp=temp->left;
			else if(temp->right==NULL){
				                      temp->right=newnode;
								      flag=0;
			                         }
               else temp=temp->right;									 
	 }
   }
   return root;
}

void iter_inorder(TREE *temp)
{
   while(temp!=NULL)
   {  push(temp);
      temp=temp->left;
   }
   
   while(top!=-1)
   {
	   temp=pop();
	   printf("\t%d",temp->info);
	   
	   if(temp->right!=NULL)
	   {   temp=temp->right;
	       while(temp!=NULL)
           {  push(temp);
              temp=temp->left;
           }
	  }
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
