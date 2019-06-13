///这个版本是全用函数全用Node


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this task
    int TaskName;  //task name
    int Etime; //execution time of this task
    int Rtime; // release time of this task
    int Dline; // deadline of this task
    int degree;
    struct HeapNode *child;
    struct HeapNode *parent;
    struct HeapNode *next;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
    int  size;      // count of items in the heap
    struct HeapNode *root;
} BinomialHeap;



// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // ... you need to define other parameters yourself)
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->degree = 0;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->child = NULL;
    new->parent = NULL;
    new->next = NULL;
    return new;
}



// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

HeapNode *HeapMerge(HeapNode *h1, HeapNode *h2){
    HeapNode *heap = NULL;
    HeapNode *heap_1 = NULL; ///for 1st heap
    HeapNode *heap_2 = NULL; ///for 2nd heap
    HeapNode *preh3 = NULL;
    HeapNode *h3 = NULL;
    if (h1!= NULL && h2!= NULL){
        heap_1 = h1;
        heap_2 = h2;
        while(heap_1 != NULL && heap_2 != NULL){
            if (heap_1 ->degree <= heap_2->degree){
                h3 = heap_1;
                heap_1 = heap_1->next;
            }
            else{
                h3 = heap_2;
                heap_2 = heap_2->next;
            }
            if (preh3 == NULL){
                preh3 = h3;
                heap = h3; /// store head of heap
            }
            else{
                preh3 ->next = h3;
                preh3 = h3;
            }
            if(heap_1 != NULL){
                h3 -> next = heap_1;
            }
            else{
                h3 -> next = heap_2;
            }
        }///while finish
    }
    else if (h1 == NULL){
        heap = h2;
    }
    else heap = h1;
    h1 = h2 = NULL;///release heap
    return heap;
}

void Heaplink(HeapNode *h1, HeapNode *h2){
    h1->parent = h2;
    h1->next = h2->child;
    h2->child = h1;
    h2->degree++;
}

HeapNode *HeapUnion(HeapNode *h1, HeapNode *h2){///去掉了*号
    HeapNode *heap;
    HeapNode *prev_x , *x, *next_x;
    heap = HeapMerge(h1, h2);
    if (heap == NULL)
        return heap;
    
    prev_x = NULL;
    x = heap;
    next_x = x->next;
    
    while (next_x != NULL)
    {
        if (   (x->degree != next_x->degree)
            || ((next_x->next != NULL) && (next_x->degree == next_x->next->degree)))
        {
            prev_x = x;
            x = next_x;
        }
        else if (x->key <= next_x->key)
        {
            x->next = next_x->next;
            Heaplink(next_x, x);
        }
        else
        {
            
            if (prev_x == NULL)
            {
                heap = next_x;
            }
            else
            {
                prev_x->next = next_x;
            }
            Heaplink(x, next_x);
            x = next_x;
        }
        next_x = x->next;
    }
    return heap;
    ///BinomialHeap *new_heap = newHeap();
    //new_heap->size = h1->size + h2->size;
    //new_heap->root = heap;
    //return new_heap;
}




HeapNode *InsertNode(HeapNode *h, int k, int n, int c, int r, int d){ ///insert node into *h
    HeapNode *node = newHeapNode(k,n,c,r,d);
    if (node == NULL) return h;
    ///BinomialHeap *heap = newHeap();
    ///heap->root= node;
    ///heap->size++;
    ///BinomialHeap *new_heap = HeapUnion(h, node);
    return HeapUnion(h, node);
    
}
// put the time complexity analysis for Insert() here
//time complexity = O(logn)
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    //put your code here
    T->root = InsertNode(T->root, k, n, c, r, d);
    T->size++;
}

// put your time complexity for RemoveMin() here
//time complexity = O(logn)
HeapNode *RemoveMin(BinomialHeap *T)
{
    // put your code here
    HeapNode *prey = NULL;
    HeapNode *y = NULL;
    HeapNode *x = T->root;
    int min = __INT_MAX__;
    while (x!= NULL){
        if(x->key < min){
            min = x->key;
            prey = y ;
            y = x ;
        }
        x= x->next;
    }
    if (y == NULL){
        return NULL;
    }
    if (prey == NULL){
        T->root = T->root->next; ///first node is min
    }
    else {
        prey->next = y->next;
    }
    HeapNode *h = NULL;
    HeapNode *p = NULL;
    x = y->child;
    while (x!= NULL){///
        p = x;
        x = x->next;
        p ->next = h;
        h = p;
        p -> parent = NULL;
    }
    T->root = HeapUnion(T->root,h);
    T->size--;
    return y;
}

//time complexity = O(logn)
int Min(BinomialHeap *T)
{
    // put your code here
    HeapNode *x = NULL;
    HeapNode *temp = T->root;
    int min = __INT_MAX__;
    while (temp != NULL)
    {
        if (temp->key < min)
        {
            min = temp->key;
            x = temp;
        }
        temp = temp ->next;
        
    }
    return min;
}



void PrintHeap(HeapNode *h) {  
    if (NULL == h) {
        return ;  
    }  
    HeapNode *p = h;  
   
    while (p != NULL) {  
        printf(" [");  
        printf(" %d", p->key);   
        if(NULL != p->child) {  
            PrintHeap(p->child);  
        }  
        printf("]\n ");  
   
        p = p->next;  
    }  
}    
// put your time complexity analysis for MyTaskScheduler here
// time complexity =  O(n*log n)
// Use BinomialHeap to realize Core , So time complexity of searching earist task in Core is O(logm)
int TaskScheduler(char *f1, char *f2, int m )
{
    int TaskName;
    int Etime;
    int Rtime;
    int Dline;
    int flag;
    BinomialHeap *h1 = newHeap();
    FILE *f = fopen(f1, "r");
    if (f == NULL)
    {
        printf("%s does not exist\n",f1);
        exit(1);
    }
    else{
        /// release time as key
        flag = fscanf(f,"%d %d %d %d ",&TaskName, &Etime, &Rtime, &Dline);
        while (flag != EOF)
        {
            ///printf("TaskName%d Etime%d Rtime%d Dline%d \n",TaskName, Etime, Rtime, Dline);
            ///create node here
            ///use two binomial heap-based priority queues, one priority queue with release times as keys and one priority queue with deadlines as keys).
            if (flag != 4){
                printf("input error when reading the attribute of the task %d.\n",TaskName);
                exit(1);
            }
            if (h1->size == 0){
                HeapNode *node_1 = newHeapNode(Rtime, TaskName, Etime, Rtime, Dline);
                h1->root = node_1;
                h1->size++;
            }
            else{
                Insert(h1, Rtime, TaskName, Etime, Rtime, Dline);
            }
            flag = fscanf(f,"%d %d %d %d ",&TaskName, &Etime, &Rtime, &Dline);
        }///end while
    }
    fclose(f);
    ///end open f1.txt and create heaps
     int currtime = 0;
    BinomialHeap *core = newHeap(); // set Core as a binomial heap-based priority queues
                                    // Key is the finish time of task in core
                                    // TaskName is that core name (1 - m)
    BinomialHeap *h2 = newHeap();
    
    //////start to write file
    FILE *fpWrite=fopen(f2,"w+");
    if(fpWrite==NULL) return 0;
    
    while (h1->size != 0 || h2->size != 0 ) 
    {
        int min_key = Min(h1);
        if (min_key <= currtime) //insert node from h1 to h2 until min key in h1 is higher than current.
        {
            HeapNode *node_h1 = RemoveMin(h1);
            int a = node_h1->Dline;
            int b =  node_h1->TaskName;
            int c =  node_h1->Etime;
            int d =  node_h1->Rtime;
            if (h2->size == 0)
            {
                HeapNode *new_h2_node = newHeapNode(a,b, c, d, a);
                h2->root = new_h2_node;
                h2->size++;
            }
            else
            {
                Insert(h2, a, b, c, d, a);
            
            }
            free(node_h1);
        }
        else if (min_key > currtime)//when min key in h1 is higher than current time, the task in h2 is ready to run in core, or there is a gap in core.
        {
            if (h2->size == 0) 
            {
                if (core->size == 0 ){ /// at beginning, no task run in core and no task is ready to run(h2 is empty).
                    currtime = min_key;
                }
                else///at a moment , no task is ready to run(h2 is empty).
                {                
                HeapNode *node_min_core = RemoveMin(core);
                Insert(core, min_key, node_min_core->TaskName, 0, 0, 0);
                currtime = Min(core);
                free(node_min_core);
                }
            }
            else
            {
                HeapNode *node_h2 = RemoveMin(h2);
                if (core->size < m )///initial the core with first m task With EDF
                {
                    if (core->size == 0 ) 
                    {
                        HeapNode *node_core = newHeapNode(node_h2->Etime + node_h2->Rtime ,core->size + 1,0,0,0);
                        core->root = node_core;
                        core->size++;
                        fprintf(fpWrite,"%d Core%d %d ",node_h2->TaskName, 1, node_h2->Rtime); ///Write file
                        if (node_h2->Etime+node_h2->Rtime > node_h2->Dline)
                            {
                        printf("task%d misses its deadline.\n",node_h2->TaskName);
                        fprintf(fpWrite," ..."); 
                        fclose(fpWrite);
                        return 0;
                            }
                    }
                    else {
                        Insert(core, node_h2->Etime + node_h2->Rtime, core->size + 1,0,0,0);
                        fprintf(fpWrite,"%d Core%d %d ",node_h2->TaskName, core->size, node_h2->Rtime); ///Write file
                        if (node_h2->Etime +node_h2->Rtime> node_h2->Dline)
                            {
                        printf("task%d misses its deadline.\n",node_h2->TaskName);
                        fprintf(fpWrite," ..."); 
                        fclose(fpWrite);
                        return 0;
                            }
                    }
                    if (core->size == m){
                        currtime = Min(core);
                    }
                    
                    
                }
                else
                {
                    HeapNode *node = RemoveMin(core);
                    fprintf(fpWrite,"%d Core%d %d ",node_h2->TaskName, node->TaskName, node->key); ///Write file
                    Insert(core, node->key + node_h2->Etime, node->TaskName,0,0,0);
                    if (node->key + node_h2->Etime > node_h2->Dline)
                    {
                        printf("task%d misses its deadline.\n",node_h2->TaskName);
                        fprintf(fpWrite," ..."); 
                        fclose(fpWrite);
                        return 0;
                    }
                    currtime = Min(core);
                    free(node);
                }
                free(node_h2);
            }
        }
    }
    ///fclose(f);
    fclose(fpWrite);
    return 1;
}/// end funcition



int main() //sample main for testing
{ int i;
    
    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
    
     
     ///There is a feasible schedule on 4 cores
     
     i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
     if (i==0) printf("No feasible schedule!\n");
     /// There is no feasible schedule on 3 cores
    
     i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
     if (i==0) printf("No feasible schedule!\n");
     /// There is a feasible schedule on 5 cores
     
     i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
     if (i==0) printf("No feasible schedule!\n");
     /// There is no feasible schedule on 4 cores
    
     i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
     if (i==0) printf("No feasible schedule!\n");
     /// There is no feasible schedule on 2 cores
     

     i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
     if (i==0) printf("No feasible schedule!\n");
     ///There is a feasible schedule on 2 cores
     
     
    return 0;
}


