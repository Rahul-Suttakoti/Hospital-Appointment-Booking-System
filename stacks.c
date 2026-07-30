#include <stdio.h>
#include <stdlib.h>
#define size 5

void push(int[],int* ,int);
void display(int[], int);
int pop(int[],int *);
int seek(int[],int *);

void main()
{
  int st[size],top, choice,x;	
  top=-1;
  
  while(1)
  {
  printf("Enter 1->PUSH\n2->POP\n3->SEEK\n4->DISPLAY\n5->EXIT");
  scanf("%d",&choice);
  
  switch(choice)
  {
	  case 1: {
		  printf("Enter the element to be pushed ");
		  scanf("%d",&x);
		  push(st, &top, x);
		 }break;
	  case 2: {
		       x=pop(st,&top);
			   if(x==-1)printf("Empty Stack");
			   else printf("The element popped is.. %d",x);
		       }break;
	  case 3: {
		       x=seek(st,&top);
		       printf("The element on the top of the stack is.. %d",x);
		       }break;
	  case 4: {
			 display(st,top);
		      }break;
		 default: exit(0);
  }
 }
}

void push(int s[],int *t,int x)
{
  if((*t)==size-1)printf("Stack Full");
  else {
       (*t)++;
      s[*t]=x;
    }
}

void display(int s[], int t)
{
  if(t==-1)printf("Empty Stack..");
  else {
	  for(int i=t;i>=0;i--)
		  printf("\t%d",s[i]);
  }
}

int pop(int s[],int *t)
{   int x=-1;
	if((*t)==-1)return x;
		   x=s[*t];
		   (*t)--;
	       return x;
}

int seek(int s[],int *t)
{   int x;
	   x=s[*t];
		return x;
}