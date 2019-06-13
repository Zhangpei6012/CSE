#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;
	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

//The function declaration..
bool isPresent(DLList *u,int data);
void append(DLList *u,int data);


// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// time complexity of input from keypad part = O(n) (one while loop),time complexity of read file part = O(n) (one while loop),so
//time complexity  = O(n)
DLList *CreateDLListFromFileDlist(const char *filename)
{
 // put your code here

 DLList *res = newDLList();
 if ((strcmp(filename, "stdin"))==0){
 DLListNode *pre = NULL;
 int i;
 char buffer[100];
 while(1){
    if(fgets(buffer,sizeof(buffer),stdin)== NULL){
         fprintf(stderr,"error\n");
         exit(EXIT_FAILURE);
     }
     if (sscanf(buffer,"%d",&i)==1){
        DLListNode *node = newDLListNode(i);
         if (res->first == NULL) {
             res->first = node;
             pre = node;
             res->size++;
             continue;
         }
        pre->next = node;
        node->prev = pre;
	    pre = node;
        res->size++;
     }
     else{break;}
 }
     res->last = pre;
 return res;
 }


FILE *file = fopen(filename,"r");
int i;
if (file == NULL){//input from file
	  printf("open file %s failed!\n", filename);
	  return 0;
}
else{
fscanf(file,"%d",&i);
	while (!feof(file)){
        append(res,i);
        fscanf(file,"%d",&i);
	}
}
fclose(file);
return res;
}
//}
// clone a DLList
// put your time complexity analysis for cloneList() here
//there is one while loop in function ,so
//time complexity = O(n)
DLList *cloneList(DLList *u)
{
 // put your code here
    int i = 0;
DLListNode *p= u->first;
DLListNode *clonehead = NULL;
DLListNode *pre = NULL;
while (p!= NULL){
    DLListNode *node= newDLListNode(p->value);
    node->next = NULL;
	if (clonehead == NULL){
        clonehead = node;
	}
	else{
        pre->next = node;
	}
    node->prev = pre;
	pre = node;
    p = p->next;
    i++;
}
    DLList *clonelist = newDLList();
    clonelist->first = clonehead;
    clonelist->last = pre;
    clonelist->size = i;
    return clonelist;
}


// compute the union of two DLLists u and v
//there is one while loop in function ,and in each loop ,function append() have one while loop ,so
//time complexity = O(n^2)
DLList *setUnion(DLList *u, DLList *v)
{
 // put your code here
    DLList *res = cloneList(u);
    DLListNode *temp = v->first;
 while (temp != NULL){
     if (!isPresent(res,temp->value)){
         append(res,temp->value);
		}
     temp = temp->next;
 }
 return res;
}
// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
//there is one while loop in function ,and in each loop ,function append() have one while loop ,so
//time complexity = O(n^2)
DLList *setIntersection(DLList *u, DLList *v)
{
  // put your code here
    DLList *res = newDLList();
    DLListNode *temp = u->first;
  while (temp!= NULL){
      int i = temp->value;
      if (isPresent(v,i)){
          append(res,i);
          }
      temp = temp->next;
      
    }
    return res;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
//there is one while loop in function ,so  
//time complexity = O(n)
void freeDLList(DLList *L)
{
// put your code here
DLListNode *e = L->first;
DLListNode *temp = NULL;
while(e!= NULL){
    temp = e->next;
	free(e);
    if (temp != NULL) temp->prev = NULL;
	e = temp;
}	
}


// display items of a DLList
// put your time complexity analysis for printDDList() here
//time complexity = O(n)
void printDLList(DLList *u){
 // put your code here
 DLListNode *e = u->first;
 while(e != NULL){
	 printf("%d\n",e->value);
	 e = e->next;
 }
     printf("\n");
}
//A function to make sure whether or not the node is in linked list
bool isPresent(DLList *u,int data){
    DLListNode *node = u->first;
	while(node != NULL){
        if (node->value == data)	return 1;
        node = node->next;
	}
	return 0;
}
///append node at the last of DLL
void append(DLList *u,int data){
    DLListNode *node = newDLListNode(data);
    node->next =NULL;
    if (u->first == NULL){
        u->first = node ;
        u->last = node ;
        u->size++;
        return;
    }
    u->last->next = node;
    node->prev = u->last;
    u->last = node;
    u->size++;
    return;
}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");

 printDLList(list1);
 list2=CreateDLListFromFileDlist("File2.txt");

 printDLList(list2);

 list3=setUnion(list1, list2);

 printDLList(list3);

 list4=setIntersection(list1, list2);

 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");

 list3=cloneList(list1);
 printDLList(list3);
 list4=cloneList(list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);
 
 return 0; 
}

