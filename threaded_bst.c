#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct thtree{
	int info;
	struct thtree *left,*right;
	bool lthread,rthread;
    }THTREE;
	
void inorder(THTREE *);
THTREE* insert(THTREE *,int);	
THTREE* inordersuccessor(THTREE*);

void main()
{ THTREE *root;
  int x,choice;
  root=NULL;
  
  while(1)  
  {
  printf("Enter 1->Insertion\n2->INORDER\n3->EXIT\n");
  scanf("%d", &choice);
  switch(choice)
  {
	  case 1: {
		      printf("Enter the value to be inserted");
		      scanf("%d", &x);
		      root=insert(root,x);	
	          }break;
	  case 2: {
		      inorder(root);
	          }break;
	  default: exit(0);
    }
  }
}

THTREE* insert(THTREE *t,int x)	
{
	THTREE *temp,*curr;
	curr=t;
	
	temp =(THTREE *)(malloc(sizeof(THTREE)));
	temp->info =x;
    temp->left=NULL;
	temp->right=NULL;
	temp->lthread=true;
	temp->rthread=true;
	
	if(curr==NULL) return temp;
	
		  while(curr!=NULL)
		  {
			if(x<curr->info)  
		        if(curr->lthread==false)curr=curr->left;
				else break;
			else if(curr->rthread==false)curr=curr->right;
				 else break;
		}
		//Assign the node
    if(x<curr->info)
	{
	  temp->left=curr->left;
	  temp->right=curr;
	  curr->left=temp;
	  curr->lthread=false;
	}	  
   else {
	  temp->right=curr->right;
	  temp->left=curr;
	  curr->right=temp;
	  curr->rthread=false;	    
   }
	return t;
}

void inorder(THTREE *t)
{
	 THTREE *curr;
     printf("Inorder Tree Traversal..");
     if(t==NULL)printf("Empty Tree..");
	 else{curr=t;
	   while(curr->lthread==false)
		    curr=curr->left;	 
		
		while(curr!=NULL)
		{
		 printf("\t%d",curr->info);
		 curr=inordersuccessor(curr);
		}
	 }
}

THTREE* inordersuccessor(THTREE *t)
{ 
  if(t->rthread==true)
	  return t->right;
  t=t->right;
  while(t->lthread==false)
	  t=t->left;
	
	return t;
	
}

