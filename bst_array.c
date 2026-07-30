#include <stdio.h>
#include <stdlib.h>

void bst_arr_insert(int);
void preorder(int);
void postorder(int);
void inorder(int);

int t[20];

void main()
 {
 
   int choice,x,i=0;
   
   while(1)
   {
   printf("Enter 1->Insert\n2->PREORDER\n3->INORDER\n4->POSTORDER\n5->EXIT\n");     
   scanf("%d", &choice);
   switch(choice)
   {
   case 1:{
          printf("Enter the value of to insert..");
          scanf("%d", &x);
          bst_arr_insert(x);
          }break;
   case 2:{
	       preorder(i);
          }break;
   case 3:{
          inorder(i);
          }break;
	case 4:{
	       postorder(i);
           }break;	
  default: exit(0);
	 
   }
  }
 }
 
 void bst_arr_insert(int x)
 { int i=0;
	 while(t[i]!=0)
	 {
	 if(x<t[i])i=2*i+1;
	 else if(x>t[i])i=2*i+2;
	 }
	  t[i]=x;
 }
 
 void inorder(int i)
{
	if(t[i]!=0)
	{
	  inorder(2*i+1);
	  printf("\t%d",t[i]);
	  inorder(2*i+2);
	}
}

void preorder(int i)
{
	if(t[i]!=0)
	{
	  printf("\t%d",t[i]);
	  preorder(2*i+1);
	  preorder(2*i+2);
	}
}

void postorder(int i)
{
	if(t[i]!=0)
	{
	  postorder(2*i+1);
	  postorder(2*i+2);
	  printf("\t%d",t[i]);
	}
}