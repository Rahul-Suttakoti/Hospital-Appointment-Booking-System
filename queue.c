
#include <stdio.h>
#include <stdlib.h>
#define size 5

void enque(int[],int*,int*,int);
void display(int[],int,int);
int deque(int [],int*,int*);
int isqfull(int);
int isqempty(int,int);


void main()
{ int que[size],choice,x,front,rear;
   front=-1;
   rear =-1;
   
	while(1)
	{
	printf("\nEnter 1->ENQUE\n2->DISPLAY\n3->DEQUE\n4->EXIT..");
	scanf("%d",&choice);
	switch(choice)
	{
	  case 1: {
		  printf("Enter the element ");
		  scanf("%d",&x);
		  enque(que,&front,&rear,x);
	         }break;
	  case 2:{ printf("The Queue contains...");
	          display(que,front,rear);
	         }break;
	  case 3:{
		      if(isqempty(front,rear))printf("Empty Queue");
			  else {x=deque(que,&front,&rear);
			        printf("The element deleted is..%d ",x);
			       }
	         }break;
	default: exit(0);
  	}
  }
}

void enque(int q[],int *f,int *r,int x)
{
	if(isqfull(*r))printf("Queue Full");
	else {
		   if((*f==-1 && *r==-1))++(*f);
		   ++(*r);
		   q[*r]=x;
	     }
}

int isqfull(int r)
{
	if(r==size-1)return 1;
	return 0;
}

int deque(int q[],int *f,int *r)
{ int x;
		     x=q[*f];
			 (*f)++;	
			 return x;
}

int isqempty(int f,int r)
{
	if((f==-1&& r==-1) || f>r ) return 1;
	return 0;
}

void display(int q[],int f,int r)
{ int i;
	if(isqempty(f,r))printf("Empty Queue");
	else {
		for(i=f;i<=r;i++)
			printf(" \t %d",q[i]);
	}
}
