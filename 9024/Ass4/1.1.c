#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "Bioheap.c"


#define INF 65535
// A vertex is a 2D point
typedef struct Vertex { 
	int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;
///###########################################
typedef struct EdgeNode {
    Edge *e;
    int length;
    //int des_index;
    struct EdgeNode *next_edge;
}EdgeNode;

typedef struct VertexNode {
	Vertex *v;
	//struct VertexNode *next_vex;
	EdgeNode *Edgehead;
} VertexNode;


///###########################################
typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode *vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

////struct of BioHeap


Edge *newEdge(Vertex *v, Vertex *w){
	Edge *new;
	new = malloc(sizeof(Edge));
	assert(new != NULL);
	new->p1 = v;
	new->p2 = w;
	return new;
}

int compute_length(Vertex *v, Vertex *w){
	int length = pow((v->x - w->x),2)+ pow((v->y - w->y),2);
	return length;
}

EdgeNode *newEdgeNode(Vertex *v, Vertex *w){
    EdgeNode *new;
    new = malloc(sizeof(EdgeNode));
    assert(new != NULL);
    new->e = newEdge(v, w);
    new->length = compute_length(v, w);
    //new->des_index = -1;
	new->next_edge = NULL;
    return new;
}

VertexNode *newVertexNode(Vertex *v){
    VertexNode *new;
    new = malloc(sizeof(VertexNode));
    assert(new != NULL);
    new->v = v;
    new->Edgehead = NULL;
    ///new->next_vex = NULL;
    return new;
}

int compare_point(Vertex *v, Vertex *w);

int LocateVex(Graph g, Vertex *v)
{
    int i;
    for(i = 0;i< g->nV;i++)
    {
        //if (g->vertices[i].v->x == v->x &&  g->vertices[i].v->y == v->y)
        if (compare_point(g->vertices[i].v, v))
            return i;
    }
    return -1; ////-1代表点不在表中
    
}

// Add the time complexity analysis of CreateEmptyGraph() here
Graph CreateEmptyGraph()
{
	int i;
    GraphRep *g;
	g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = 0;
	g->nE = 0;
	g->vertices = malloc(0 * sizeof(VertexNode));
	assert(g->vertices != NULL);
	for(i = 0; i<0;i++)
		g->vertices[i].Edgehead = NULL;
	return g;
}
///

int inLL(Graph g, Vertex *v, Vertex *w)
{
	int i = LocateVex(g, v);
    
    EdgeNode *temp = g->vertices[i].Edgehead;
    while(temp != NULL)
        {
            if ((temp->e->p2->x == w->x)&&(temp->e->p2->y == w->y))
            {
                return 1;
            }
            temp = temp->next_edge;
        }
	return 0;
}

int compare_point(Vertex *v, Vertex *w)
{
	
		if ((v->x == w->x)&&(v->y == w->y))
			return 1;
		else return 0;
	

}
void insertLL(Graph g, Vertex *v, Vertex *w) ///v是起始点
{
    int i = LocateVex(g, v);
	EdgeNode *new = newEdgeNode(v, w);
	EdgeNode *temp = g->vertices[i].Edgehead;
    EdgeNode *pre = NULL;
    if (temp == NULL)
    {
         g->vertices[i].Edgehead = new;
    }
    else
	{
        while (temp != NULL)
            {
                pre = temp;
                temp = temp->next_edge;
            }
        pre->next_edge = new;
    }
}
void deleteLL(Graph g, Vertex *v, Vertex *w) ///v是起始点
{
    EdgeNode *temp,*pre = NULL;
    int i = LocateVex(g, v);
    
    temp = g->vertices[i].Edgehead;
	while ((temp->e->p2->x != w->x &&temp->e->p2->y != w->y) &&temp->next_edge != NULL)
	{
		pre = temp;
		temp = temp->next_edge;
    }
	if (temp->e->p2->x == w->x &&temp->e->p2->y == w->y )
	{
		if (pre  ==  NULL) g->vertices[i].Edgehead = temp->next_edge;
		else pre->next_edge = temp ->next_edge;
		free(temp);
	}
	else printf("not found deleted edge");
}




int InsertEdge(Graph g, Edge *e)
// Add the time complexity analysis of InsertEdge() here
{
    assert(g!= NULL && e );
	Vertex *v = e->p1;
	Vertex *w = e->p2;
    if ((LocateVex(g,v) == -1)&&(LocateVex(g, w) == -1))////两个点均不在途中
    {
        g->vertices = realloc(g->vertices,(g->nV+2)*sizeof(VertexNode));
        g->vertices[g->nV] = *newVertexNode(v);
        g->vertices[g->nV+1] = *newVertexNode(w);
        g->nV += 2;
    }
    else if((LocateVex(g,v) == -1) && (LocateVex(g, w) != -1))///有一个点不在图中
    {
        g->vertices = realloc(g->vertices,(g->nV+1)*sizeof(VertexNode));
        g->vertices[g->nV] = *newVertexNode(v);
        g->nV += 1;
    }
    else if((LocateVex(g,v) != -1) && (LocateVex(g, w) == -1))
    {
        g->vertices = realloc(g->vertices,(g->nV+1)*sizeof(VertexNode));
        g->vertices[g->nV] = *newVertexNode(w);
        g->nV += 1;
    }
    
	if (inLL(g, v, w) == 0)
	{
        int i = LocateVex(g, v);
        int j = LocateVex(g, w);

        insertLL(g, v, w);
        g->nE ++;

        insertLL(g, w, v);
        return 1;
    }
	return 0;
}

// Add the time complexity analysis of DeleteEdge() here
void DeleteEdge(Graph g, Edge *e)
{
    Vertex *v = e->p1;
    Vertex *w = e->p2;
    
    if (inLL(g, v, w) == 1)
	{
        int i = LocateVex(g, v);
        int j = LocateVex(g, w);
        int k,m;
        
        deleteLL(g, v, w);
        g->nE--;
        if (g->vertices[i].Edgehead == NULL)
        {
            for(k = i;k<g->nV-1;k++)
                {
                    g->vertices[k]=g->vertices[k+1];
                        
                }
                g->nV--;
                printf("zhe lie %d",g->nV);
            }

    /// v 是起始点
        deleteLL(g, w, v);
        if (g->vertices[j].Edgehead == NULL)
        {
            for(m = j;m<g->nV-1;m++)
            {
                g->vertices[m]=g->vertices[m+1];
            }
            g->nV--;
            printf("na lie %d",g->nV);
        }
    }

} /// w 是起始点


void DFS(Graph g, int i, int Visited[], int flag)
{
    EdgeNode *node;
    
    Visited[i] = 1;
    if (flag)
    {
        printf("(%d, %d)",g->vertices[i].v->x,g->vertices[i].v->y);
        flag = 0;
    }
    else
        printf(",(%d, %d)",g->vertices[i].v->x,g->vertices[i].v->y);
    node = g->vertices[i].Edgehead;
    while(node != NULL)
    {
        int j = LocateVex(g, node->e->p2);
        if (!Visited[j])
            DFS(g, j, Visited,flag);
        node = node->next_edge;
    }
    
    
}
// Add the time complexity analysis of ReachableVertices() here
void ReachableVertices(Graph g, Vertex *v)
{
    printf("Here is reachable vertices:\n");
	int v_index = LocateVex(g,v);
    int visited[g->nV];
    for(int i = 0; i< g->nV; i++)
        visited[i] = 0;
    DFS(g, v_index, visited,1);
    printf("\n");
}

// Add the time complexity analysis ofH ShortestPath() here
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	int index_u = LocateVex(g, u);
	int index_w = LocateVex(g, v);
	if (index_u || index_w)
	{
		return;
	}

	int i,j,k;
	int path[g->nV];

	HeapNode *dest = NULL;
	int min;
	int temp;
	int flag[g->nV];
    // 初始化
    for (i = 0; i < g->nV; i++)
    {
		if(i == index_u)
		{
			flag[i] = 0;

		}
		else
			flag[i] = INF;
		if (dest == NULL)
		{
			dest = newHeapNode(flag[i],i);
		}
		else
			dest = InsertNode(dest,flag[i],i);
    }
	while (dest != NULL)
	{
		HeapNode *x = RemoveMin(dest);
        EdgeNode *t =  g->vertices[x->des].Edgehead;
		while(t)
		{
            int t_index = LocateVex(g, t->e->p2);
			if ((flag[x->des] + t->length) < flag[t_index])////////这里还要改des_index
			{
                path[t_index] = x->des;
				flag[t_index] = flag[x->des] + t->length;
				///change flag[z] the key of vertex in Q
			}
		}
	}

}


// Add the time complexity analysis of FreeGraph() here
void FreeGraph(Graph g)
{
	int i;
	for(i = 0;i<g->nV;i++)
	{
        EdgeNode *p = g->vertices[i].Edgehead;
		EdgeNode *temp;
        if (!p)
            return;
		while (p)
		{
			temp = p->next_edge;
			free(p);
			p = temp;
		}
		//free(g->vertices[i]);
	}
	free(g->vertices);
    free(g);
	
}



void BFS(Graph g, Vertex *v ,int visited[][g->nV])
{
    int start = LocateVex(g, v);
    int head = 0;
	int rear = 0;
	int queue[g->nV];
    //int visited = *vlist;
	//int i,j,k;
    queue[rear++] = start;
	EdgeNode *eNode;
    visited[start][start] = 1;
    //int i = 0;
    int j;
    int k;
    while (head != rear)
    {
        //for(j = head;j<rear;j++)
        //{
            j = queue[head++];
            eNode = g->vertices[j].Edgehead;
            while(eNode != NULL)
            {
                k = LocateVex(g, eNode->e->p2);
                if (visited[j][k] == 0)///edge j to k is UNEXPLORED
                {
                    if (visited[k][k] == 0)///point k is UNEXPLORED
                    {
                        visited[j][k] = 1; ///set edge to DISCOVERY
                        visited[k][j] = 1;
                        printf("(%d, %d),(%d, %d) ",g->vertices[j].v->x,g->vertices[j].v->y,g->vertices[k].v->x,g->vertices[k].v->y);
                        visited[k][k] = 1;
                        queue[rear++] = k;
                    }
                    else
                    {
                        visited[j][k] = 1; ///set edge to cross
                        visited[k][j] = 1;
                        printf("(%d, %d),(%d, %d) ",g->vertices[j].v->x,g->vertices[j].v->y,g->vertices[k].v->x,g->vertices[k].v->y);
                    }
                }
                eNode = eNode->next_edge;
            }
        
    }

}

// Add the time complexity analysis of ShowGraph() here
void ShowGraph(Graph g)
{
    int i;
    int j;
    int visited[g->nV][g->nV];
    
    //printf("%d\n",g->nV);
    //printf("%d\n",g->nE);
    printf("Here is Graph:\n");
    for(i = 0; i<g->nV; i++)
        for(j = 0; j<g->nV; j++)
        {
            visited[i][j] =0;
            //printf("visit %d %d %d\n",i,j,visited[i][j]);
        }
    
    for (i = 0; i< g->nV; i++)
	{
		if (!visited[i][i])
		{
            BFS(g,g->vertices[i].v, visited);
		}
	}
    printf("\n");
}

int main() //sample main for testing 
{ 
 Graph g1;
 Edge *e_ptr; 
 Vertex *v1, *v2;
  
 // Create an empty graph g1;
 g1=CreateEmptyGraph();
  
 // Create first connected component 
 // Insert edge (0,0)-(0,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=0;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0, 10)-(10, 10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=10;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,10)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5, 4)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=4;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5,6)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=6;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10,10)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=10;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10, 6)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=6;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Create second connected component
 // Insert edge (20,4)-(20,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=4;
 v2->x=20;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (20,10)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=10;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (25,5)-(30,10) 	
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=25;
 v1->y=5;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 
 
 //Display graph g1

 ShowGraph(g1);
	
 // Find the shortest path between (0,0) and (10,6) 
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);
	  


 // Delete edge (0,0)-(5, 6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2; 	 
 DeleteEdge(g1, e_ptr);
 free(e_ptr);
 free(v1);
 free(v2);
 	 
 // Display graph g1


 ShowGraph(g1);
	
 // Find the shortest path between (0,0) and (10,6) 
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6; 
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);
 
 // Find the shortest path between (0,0) and (25,5)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=25;
 v2->y=5;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);	
 
 // Find reachable vertices of (0,0)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=0;
 v1->y=0;
 ReachableVertices(g1, v1);
 free(v1);
 
 // Find reachable vertices of (20,4)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=20;
 v1->y=4;
 ReachableVertices(g1, v1);
 free(v1);
 
 // Free graph g1
 FreeGraph(g1);
 
 return 0; 
}
