#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int evaluate(char[]);
void push(int);
int pop();
int isoperator(char);

int s[10];
int top=-1;

void main()
{
  char postfix[10];
  int x;
  printf("Enter a valid postfix expression");
  scanf("%s",postfix);
  x=evaluate(postfix);
  printf("The Result is....%d",x);  
}

int evaluate(char postfix[])
{ int i=0,x,y,z;
  char ch;
  
  while((ch=postfix[i++])!='\0')
  { if(isalnum(ch)) push(ch-'0');
	else if(isoperator(ch))
    {
		x=pop();
		y=pop();
	
		switch(ch)
		{
	  case '+':{
		         z=x+y;
	           } break;
	  case '-':{
		         z=y-x;
	           } break;
	  case '*':{
		         z=x*y;
	           } break;
	  case '/':{
		         z=y/x;
	           } break;
	  case '^':{
		         z=pow(y,x);
	           } break;
      }
	  push(z);
	}
  }
  return pop();
}

void push(int x)
{
	s[++top]= x;
}
	
int pop()
{
  return s[top--];
}

int isoperator(char ch)
{
  switch(ch)
  {
   case '+':
   case '-': 
   case '*':
   case '/':
   case '^':return 1;   
  }
  return 0;
}