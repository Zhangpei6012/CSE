


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define SWAP(x,y,t) ((t) = (x), (x) =(y),(y) = (y))

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this task
    int des;
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
HeapNode *newHeapNode(int k,int i)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // ... you need to define other parameters yourself)
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->des = i;/////目标编号
    new->degree = 0;
    
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




HeapNode *InsertNode(HeapNode *h, int k, int i){ ///insert node into *h
    HeapNode *node = newHeapNode(k, i);
    if (node == NULL) return h;
    ///BinomialHeap *heap = newHeap();
    ///heap->root= node;
    ///heap->size++;
    ///BinomialHeap *new_heap = HeapUnion(h, node);
    return HeapUnion(h, node);
    
}



// put your time complexity for RemoveMin() here
//time complexity = O(logn)
HeapNode *RemoveMin(HeapNode *T)
{
    // put your code here
    HeapNode *prey = NULL;
    HeapNode *y = NULL;
    HeapNode *x = T;
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
        T = T -> next; 
    else {
        prey->next = y->next;
    }
    HeapNode *h = NULL;
    HeapNode *p = NULL;
    x = y->child;
    while (x!= NULL)
    {///
        p = x;
        x = x->next;
        p ->next = h;
        h = p;
        p -> parent = NULL;
    }
    T = HeapUnion(T,h);
    return y;
}

//time complexity = O(logn)
int Min(HeapNode *T)
{
    // put your code here
    HeapNode *x = NULL;
    HeapNode *temp = T;
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





void PrintBinHeap(HeapNode *h) {
    if (NULL == h) {
        return ;
    }
    HeapNode *p = h;
    
    while (p != NULL) {
        printf(" (");
        printf(" %d index = %d", p->key,p->des);
        if(NULL != p->child) {
            PrintBinHeap(p->child);
        }
        printf(") ");
        
        p = p->next;
    }
}




