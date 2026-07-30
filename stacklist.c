
#include <stdio.h>
#include <stdlib.h>
#define size 5

typedef struct node {
                    int info;
	                struct node *next;
                   }NODE;
				   
 NODE* push(NODE *,int);
 void display(NODE *);
 NODE* pop(NODE *);
  int count;
  
void main()
 {
   NODE *list, *newnode;
   int choice,x;
   list=NULL;
  count =0;
   while(1)
   {
   printf("Enter 1->PUSH\n2->DISPLAY\n3->POP\n4->Exit");
   scanf("%d", &choice);
   switch(choice)
   {
   case 1:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          list=push(list,x);
          }break;
   case 2:{
	      display(list);
          }break;
   case 3:{
	       list=pop(list);
           }break;	  
   default: exit(0);
	 
   }
  }
 }
 
 NODE* push(NODE *list, int x)
 {
	 NODE *newnode;
	 	 
     newnode = (NODE *)(malloc(sizeof(NODE)));
	 newnode->info=x;
	 newnode->next=NULL;
	 if(count==size)printf("Stack FULL");
	 else if(list==NULL) list=newnode;
	 else {
		 newnode->next=list;
		 list=newnode;
	 }
	 count++;
	 return list;
 }
 
 void display(NODE *list)
 {  NODE *temp;int i;
     temp=list;
	 if(count==0){printf("Empty STACK..");
	                return;
				   }
	 i=count;
	 while(i>=0)
	 {
	   printf("\t%d", temp->info);	
       temp=temp->next;	  i--; 
	 }
 }
 
 NODE* pop(NODE *list)
 {   NODE *temp;
	 if(count==0) printf("Empty Stack..");
	
	 else { temp=list;
		   list=list->next;
		   free(temp); 
	      }
		  count--;
	 return list;
 }
 
  
