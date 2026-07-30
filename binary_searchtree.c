#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
                    int info;
	                struct tree *left;
					struct tree *right;
                   }TREE;
				   
TREE* insert(TREE *,int );
void preorder(TREE *);
void postorder(TREE *);
void inorder(TREE*);
void numofnodes(TREE *);
int search(TREE *,int );
TREE* bst_delete( TREE *,int );

int count=0;
				   
void main()
 {
   TREE *root;
   int choice,x;
   root=NULL;
   
   while(1)
   {
   printf("Enter 1->Insert\n2->PREORDER\n3->INORDER\n4->POSTORDER\n");  
   printf("Enter 5->Search\n6->EXIT\n");
   
   scanf("%d", &choice);
   switch(choice)
   {
   case 1:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          root=insert(root,x);
          }break;
   case 2:{
	       preorder(root);
          }break;
   case 3:{
          inorder(root);
          }break;
	case 4:{
	       postorder(root);
           }break;	
	case 5:{
	       numofnodes(root);
		   printf("The number of nodes in the tree are  %d",count);
           }break;	
    case 6:{
          printf("Enter the value to Search..");
          scanf("%d", &x);
          x=search(root,x);
		  if(x==-1)printf("Element Not Found");
		  else printf("Element Found");
          }break;		   
	case 7:{ x=-1;
          printf("Enter the value to Search and Delete.");
          scanf("%d", &x);
          root=bst_delete(root,x);
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

void inorder(TREE *root)
{
	if(root)
	{
	  inorder(root->left);
	  printf("\t%d",root->info);
	  inorder(root->right);
	}
}

void preorder(TREE *root)
{
	if(root)
	{
	  printf("\t%d",root->info);
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
	  printf("\t%d",root->info);
	}
}

void numofnodes(TREE *root)
{
	if(root)
	{
	  count++;
	  numofnodes(root->left);
	  numofnodes(root->right);
	}
}

void count_leafnodes(TREE *root)
{
	if(root)
	{
	  if(root->left==NULL && root->right==NULL)count++;
	  count_leafnodes(root->left);
	  count_leafnodes(root->right);
	}
}

int search(TREE *root,int key)
{   TREE *temp=root;
	while(temp!=NULL)
	{
     if(key==temp->info)return key;
     if(key<temp->info)temp=temp->left;
     else temp=temp->right;	 
	}
	return -1;
}

TREE* bst_delete( TREE *root,int key)
{ int x=-1;
	TREE *temp=root,*parent=NULL,*q=NULL,*insucc;
	while(temp!=NULL && key!=temp->info)
	{ parent=temp;
      if(key<temp->info)temp=temp->left;
      else temp=temp->right;	 
	}
	if(temp==NULL)printf("Element not found");
	//single child node 
	else if(temp->left=NULL)q=temp->right;
	else if(temp->right==NULL)q=temp->left;
	// two child case
	else {insucc=temp->right;
	      while(insucc->left!=NULL)
			  insucc=insucc->left;
		  
		  insucc->left=temp->left;
		  q=temp->right;
	     }
		 
		 if(parent==NULL)root=q;
		 else if(parent->left==temp)parent->left=q;
		 else if(parent->right==temp) parent->right=q;
		 free(temp);
		 
		 return root;
}
