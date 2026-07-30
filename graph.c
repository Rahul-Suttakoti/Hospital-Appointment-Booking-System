#include <stdio.h>
#include <stdlib.h>

void creategraph();
void display(int);
void dfs(int,int);

int adj[10][10];
int visit[10];

void main()
{ int n,v;
  printf("Enter the number of vertices");
  scanf("%d",&n);
  
  for(int k=1;k<=n;k++)
	  visit[k]=0;

  creategraph();

  display(n);
  printf("Enter the start vertex");
  scanf("%d",&v);
  
  printf("\nGraph Traversal.. DFS ..");
  dfs(v,n);
}

void creategraph()
{ int i,j;
  while(1)
  {
  printf("Enter the Source & Destination Vertex");
  scanf("%d%d",&i,&j);
  if(i==0 && j==0) break;
   {
    adj[i][j]=1;
   }
  }
}

void display(int n)
{ int p,q;
	for(p=1;p<=n;p++)
	{ for(q=1;q<=n;q++)
		printf("\t%d",adj[p][q]);
      printf("\n");
    }
}

void dfs(int v,int n)
{ int w;
	printf("\t%d",v);
	visit[v]=1;
	for(w=1;w<=n;w++)
	if(visit[w]==0 && adj[v][w]==1)
		dfs(w,n);
}