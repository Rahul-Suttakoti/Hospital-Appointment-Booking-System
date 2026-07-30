
#include <stdio.h>
#include <stdlib.h>

typedef struct dnode {
                    int info;
	                struct dnode *left;
					struct dnode *right;
                   }DNODE;

DNODE* frontins(DNODE *, int);	
DNODE* endins(DNODE *, int); 
void display(DNODE *);
DNODE* frontdel(DNODE *);
DNODE* enddel(DNODE *);
DNODE* succnodeins(DNODE *,int,int);
DNODE* succnodedel(DNODE *,int);

				   
void main()
 {
   DNODE *dlist;
   int choice,x,succ;
   dlist=NULL;
   
   while(1)
   {
   printf("Enter 1->Front Insertion\n2->Display\n3->End Insertion\n");  
   printf("Enter 4->Front deletion\n5->End deletion\n6->Insert node given the successor node info\n");
   printf("Enter 7->Deletion of a node given the successor node information\n8->Ordered Insertion\n9->Exit");
   scanf("%d", &choice);
   switch(choice)
   {
   case 1:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          dlist=frontins(dlist,x);
          }break;
   case 2:{
	      display(dlist);
          }break;
   case 3:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          dlist=endins(dlist,x);
          }break;
	case 4:{
	        dlist=frontdel(dlist);
           }break;	  
	case 5:{
	        dlist=enddel(dlist);
           }break;	
    case 6:{
          printf("Enter the value and succ information");
          scanf("%d %d", &x,&succ);
          dlist=succnodeins(dlist,x,succ);
          }break;
	case 7:{
          printf("Enter the successor node information");
          scanf("%d", &succ);
          dlist=succnodedel(dlist,succ);
          }break;
	/*case 8:{
          printf("Enter the value");
          scanf("%d", &x);
          list=orderedins(list,x);
          }break;			  */
  default: exit(0);
	 
   }
  }
 }
 
 DNODE* frontins(DNODE *dlist, int x)
 {
	 DNODE *newnode;
	 
     newnode = (DNODE *)(malloc(sizeof(DNODE)));
	 newnode->info=x;
	 newnode->left=NULL;
	 newnode->right=NULL;
	 
	 if(dlist==NULL) dlist=newnode;
	 else {
		 newnode->right=dlist;
		 dlist->left=newnode;		 
		 dlist=newnode;
	 }
	 return dlist;
 }
 
 void display(DNODE *dlist)
 {  DNODE *temp;
     temp=dlist;
	 if(dlist==NULL){printf("Empty List..");
	                return;
				   }
	 while(temp!=NULL)
	 {
	   printf("\t%d", temp->info);	
       temp=temp->right;	   
	 }
 }
 
 DNODE* endins(DNODE *dlist, int x)
 {
	 DNODE *newnode,*temp;
	 
     newnode = (DNODE *)(malloc(sizeof(DNODE)));
	 newnode->info=x;
	 newnode->left=NULL;
	 newnode->right=NULL;
	 
	 if(dlist==NULL) dlist=newnode;
	 else { temp=dlist;
	      while(temp->right!=NULL)
			temp=temp->right;  
		  temp->right=newnode;
		  newnode->left=temp;
	 }
	 return dlist;
 }
 
 
 DNODE* frontdel(DNODE *dlist)
 {   DNODE *temp;
	 if(dlist==NULL) printf("Empty List..");
	 else if(dlist->right==NULL){ 
	                             dlist=NULL;
								 free(temp);
	                            }
	 else { temp=dlist;
		   dlist=dlist->right;
		   dlist->left=NULL;
		   free(temp); 
	      }
	 return dlist;
 }
 
  DNODE* enddel(DNODE *dlist)
 {
	 DNODE *temp;
	 temp=dlist; 
	 if(dlist==NULL) printf("Empty List..");
	 else if(dlist->right==NULL)dlist=NULL;
	 else { 
	      while(temp->right!=NULL)
		   temp=temp->right;  
	   
		  (temp->left)->right=NULL;
	 }
	 free(temp);
	 return dlist;
 }
 
 
 DNODE* succnodeins(DNODE *dlist, int x,int succ)
 {
	 DNODE *newnode,*temp;
	 
	 newnode = (DNODE *)(malloc(sizeof(DNODE)));
	 newnode->info=x;
	 newnode->left=NULL;
	 newnode->right=NULL;
	 temp=dlist;
	 
	 if(dlist==NULL) dlist=newnode;
	 else if(dlist->info==succ){
			        newnode->right=dlist;
					dlist->left=newnode;
					dlist=newnode;
		          }
	else{
		while(temp->right!=NULL && (temp->right)->info!=succ)
			  temp=temp->right;
		  
	   if(temp->info==succ)
		{
		  newnode->right=temp->right;
          newnode->left=temp;
          temp->right=newnode;
          (newnode->right)->left=newnode;	  
		}
		else printf("\nInvalid Successor Node Information..");
	 }
	 return dlist;
 }
 
 DNODE* succnodedel(DNODE *dlist,int succ)
 {
	 DNODE *temp;
	 
	 temp=dlist;
	 
	 if(dlist==NULL) printf("Empty List");
	 else if(dlist->right==NULL)printf("does not have a successor node");
	 else if((dlist->right)->info==succ)
	              {
			        dlist=dlist->right;
					dlist->left=NULL;
					free(temp);
		          }
	else{
		while(temp->right!=NULL && (temp->right)->info!=succ)
			  temp=temp->right;
		  
	   if(temp->info==succ)
		{
		  (temp->left)->right=temp->right;
		  (temp->right)->left=temp->left;
		}
		else printf("\nInvalid Successor Node Information..");
	 }
	 return dlist;
 }
