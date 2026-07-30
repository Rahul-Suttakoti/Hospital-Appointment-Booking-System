#include <stdio.h>
#include <stdlib.h>

void heapify(int *);
void maxdel(int );
void adjust(int);


int h[100];

void main()
{ int n,p,count;
  printf("Enter the number of elements..");
  scanf("%d",&n);
  count=n;
  printf("Enter the elements..");
  for(p=0;p<n;p++)
	scanf("%d",&h[p]);  
 
   heapify(&count);
   
  printf("\nHeap elements are ..");
  for(p=0;p<n;p++)
	printf("\t%d",h[p]);  

  maxdel(count);
 
 printf("\nHeap elements after deletion is ..");
  for(p=0;p<n;p++)
	printf("\t%d",h[p]);  

}

void heapify(int *count)
{ int i,j,k,key;
  for(k=1;k<*count;k++)
  { i=k;
    key=h[i];
	j=(i-1)/2;
	
	while(i>0 && h[j]<key)
	{
		h[i]=h[j];
		i=j;
		j=(i-1)/2;
	}
	 h[i]=key;
  }
}
void maxdel(int count)
{
   int max;
   max=h[0];
   h[0]=h[count-1];
   count--;
   printf("The max element in the heap is..%d",max);
   adjust(count);
 
}

void adjust(int count)
{
  int i,j,key;
  printf("Hello..%d",count);
  while(i<count) // as long as left child exists
  {
	  if(i+1<count) //if right child exists
	  { if(h[i+1]>h[i])i++;
	      if(key<h[i])
	      {
		    h[j]=h[i];
		    j=i;
		    i=2*j+1;
	      }
	  }
		 else break;
	   }
  h[j]=key;
}