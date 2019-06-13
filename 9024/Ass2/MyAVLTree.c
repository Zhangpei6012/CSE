#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
int flag=0;
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int  value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

int InsertNode(AVLTree *T, int k, int v);

// put your time complexity analysis of CreateAVLTree() here
// time complexity of input from keypad part = O(nlogn) (one while loop,and each loop have one InsertNode function)
//,time complexity of read file part = O(nlohn) (one while loop,and each loop have one InsertNode function),so
//time complexity  = O(nlogn)
AVLTree *CreateAVLTree(const char *filename)
{
    AVLTree *tree = newAVLTree();
    char strLine[100];
    char b1[100] ,b2[100] ,b3[100] ,b4[100] ;
    int key,value;
    if ((strcmp(filename, "stdin"))==0){// input from keybroad
        int i=1;
        while(i){
            fgets(strLine,10, stdin);
            if (strlen(strLine) != 1)
            {
                sscanf(strLine,"(%s %s",b1,b2);
                sscanf(b1,"%[^,]",b3);
                sscanf(b2,"%[^)]",b4);
                key = atoi(b3);
                value = atoi(b4);
                if (tree->size == 0)
                {
                    AVLTreeNode *newnode = newAVLTreeNode(key,value);
                    tree->root = newnode;
                    tree->size++;
                }
                else
                {
                    InsertNode(tree,key,value);
                }
            }
            else i = 0;
            strcpy(b1,"");
            strcpy(b2,"");
            strcpy(b3,"");
            strcpy(b4,"");
        }
        return tree;
    }
        // input from file
        FILE *f = fopen(filename,"r");
        ///int key,value;
        if (f == NULL)
        {//input from file
            printf("open file %s failed!\n", filename);
            return NULL;
        }
        else{
            while(fscanf(f,"(%d, %d) ",&key,&value) != EOF)
            {
                if (tree->size == 0)
                {
                    AVLTreeNode *newnode = newAVLTreeNode(key,value);
                    tree->root = newnode;
										newnode->height = 0;
                    tree->size++;
                }
                else
                {
                    InsertNode(tree,key,value);
                }
                
            }
            fclose(f);
        }
    return tree;
 }

AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *N)
{
    if (N == NULL){
        return NULL;
    }
    AVLTreeNode *newtreenode = newAVLTreeNode(N->key, N->value);
    ///newtreenode->root = newAVLTreeNode(N->key, N->value);
    newtreenode->height = N->height;
    newtreenode->left = CloneAVLTreeNode(N->left);
    newtreenode->right = CloneAVLTreeNode(N->right);
    return newtreenode;
}
// put your time complexity analysis for CloneAVLTree() here
//O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
    if (T->root == NULL){
        return NULL;
    }
    AVLTree *newtree = newAVLTree();
    newtree->root = CloneAVLTreeNode(T->root);
    newtree->size = T->size;
    return newtree;
}

 void storeInArry(AVLTreeNode *node, int arry[], int *index)
 {
 if (node == NULL) return;
 storeInArry(node->left,arry,index);
 arry[*index] = [node->key,node->value];
 (*index)++; //increase index for next pos
 storeInArry(node->right,arry,index);
 }

AVLTree *intersectionArry(int arr1, int arr2, int m, int n)
 {
  int i = 0, j = 0; 
	AVLTree *tree = newAVLTree();
  while (i < m && j < n) 
  { 
    if (arr1[i][0] < arr2[j][0]) 
      i++; 
    else if (arr2[j][0] < arr1[i][0]) 
      j++; 
		else if (arr1[i][0]== arr2[j][0]){
			if (arr1[i][1] < arr2[j][1]) 
      i++; 
      else if (arr2[j][1] < arr1[i][1]) 
      j++; 
      else 
    { 
         if (tree->size == 0)
                {
                    AVLTreeNode *newnode = newAVLTreeNode(arr1[i][0],arr1[i][1]);
                    tree->root = newnode;
										newnode->height = 0;
                    tree->size++;
                }
         else
                {
                    InsertNode(tree,arr1[i][0],arr1[i][1]);
                }
			j++; 
      i++; 
    } 
		}
  } 
 }
 /*
 AVLTreeNode * ArrytoTree(int arr[], int start, int end)
 {
 if(start > end) return NULL;
 int mid = (start + end)/2;
 AVLTreeNode *node = newAVLTreeNode(arr[mid][0],arr[mid][1]);
 root->left = ArrytoTree(arr,start,mid-1);
 root->right = ArrytoTree(arr,mid+1,end);
 return node
 }
*/
 void InorderTravel_1(AVLTreeNode *T1,AVLTree *T2)
{
		if (T1 != NULL)
    {
        InorderTravel_1(T1->left, T2);
        InsertNode(T2,T1->key,T1->value);
        InorderTravel_1(T1->right, T2);
    }
}

 // put your time complexity for ALVTreesUNion() here
 // time complexity of cloneAVLTree = m     (size of T1)
 // time complexity of InorderTravel = n*log(m) ( travel every node in T2  and insert in T1)
 // time complexity = m + nlog(m) < O(mlog(m+n) + nlog(m+n))
 AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
 {
 //put your code here
 AVLTree *newtree = CloneAVLTree(T1);
InorderTravel_1(T2->root,newtree);
return newtree;
 }
 
 // put your time complexity for ALVTreesIntersection() here
 AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
 {
 //put your code here
int *arr1 = int [T1->size];
int i = 0;
storeInArry(T1->root,arr1,&i);

int *arr2 = int [T2->size];
int j = 0;
storeInArry(T2->root,arr2,&j);

AVLTree *tree = intersectionArry(arr1, arr2,T1->size,t2->size);
return tree;
 }
 
int Height(AVLTreeNode *T){
    if (T == NULL) return -1;
    else return T->height;
}
AVLTreeNode *LL(AVLTreeNode *k2)
{
    AVLTreeNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = MAX(Height(k2->left) , Height(k2->right))+1;
    k1->height = MAX(Height(k1->left) , Height(k1->right))+1;
    
    return k1;
}

AVLTreeNode *RR(AVLTreeNode *k1)
{
    AVLTreeNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = MAX(Height(k1->left) , Height(k1->right))+1;
    k2->height = MAX(Height(k2->left) , Height(k2->right))+1;
    
    return k2;
}

AVLTreeNode *LR(AVLTreeNode *k3)
{
    k3->left = RR(k3->left);
    return LL(k3);
}

AVLTreeNode *RL(AVLTreeNode *k3)
{
    k3->right = LL(k3->right);
    return RR(k3);
}

AVLTreeNode *__InsertNode(AVLTreeNode *T, int k, int v){
    
    if(T == NULL)
    {
        T = newAVLTreeNode(k, v);
    }
    else if(T->key > k)
    {
        T->left  = __InsertNode(T->left,k,v);
        if (Height(T->left) - Height(T->right)==2)
        {
            if (T->left->key>k)
            {
                T = LL(T);
            }
            else if(T->left->key<k)
            {
                T = LR(T);
            }
        }
    }
    else if(T->key < k)
    {
        T->right = __InsertNode(T->right,k,v);
        if (Height(T->right) - Height(T->left)==2)
        {
            if (T->right->key < k)
            {
                T = RR(T);
            }
            else if(T->left->key > k)
            {
                T = LL(T);
            }
        }
    }
    else if(T->key == k)
    {
        if(T->value > v)
        {
            T->left  = __InsertNode(T->left,k,v);
            if (Height(T->left) - Height(T->right)==2)
            {
                if (T->left->key>k)
                {
                    T = LL(T);
                }
                else if(T->left->key<k)
                {
                    T = LR(T);
                }
                else if (T->left->key == k)
                {
                    if (T->left->value>v)
                    {
                        T = LL(T);
                    }
                    else if(T->left->value<v)
                    {
                        T = LR(T);
                    }
                    
                }
            }
        }
        else if(T->value < v)///需要不需要再比较k？？
        {
            T->right = __InsertNode(T->right,k,v);
            if (Height(T->right) - Height(T->left)==2)
            {
                if (T->right->key < k)
                {
                    T = RR(T);
                }
                else if(T->left->key > k)
                {
                    T = LL(T);
                }
                else if(T->left->key == k)
                {
                    if (T->right->value < v)
                    {
                        T = RR(T);
                    }
                    else if(T->left->value > v)
                    {
                        T = LL(T);
                    }
                }
            }
        }
        else if (T->value == v)
        {
            printf("There is a duplicate node;\n");
						if (flag == 1) flag = 0;
            return T;
        }
    }
    T->height = MAX( Height(T->left),Height(T->right))+1;
    return T;
}
// put the time complexity analysis for InsertNode() here
// time complexity = O(logn)
int InsertNode(AVLTree *T, int k, int v)
{
		flag = 1;
    T->root = __InsertNode(T->root,k,v);
    //T->size += 1;
		if (flag == 0) return 0;
		else {
			T->size += 1;
			return 1;
}
}
AVLTreeNode *__DeleteNode(AVLTreeNode *T, int k, int v)
{
    {
        if(T == NULL)
        {
            printf("Don`t have this data!!!\n");
						flag = 0;
            return NULL;
        }
        
        if(T->key > k )
        {
            T->left = __DeleteNode(T->left,k,v);
            if(Height(T->right) - Height(T->left) == 2)
            {
                if(Height(T->right->left) > Height(T->right->right))
                {
                    T = RL(T);
                }
                else T = RR(T);
            }
        }
        else if(k > T->key)
        {
            T->right = __DeleteNode(T->right,k,v);
            if(Height(T->left) - Height(T->right) == 2)
            {
                if(Height(T->left->left) > Height(T->left->right))
                {
                    T = LL(T);
                }
                else T = LR(T);
            }
        }
        else if(k == T->key)
        {
            if(T->value > v )
            {
                T->left = __DeleteNode(T->left,k,v);
                if(Height(T->right) - Height(T->left) == 2)
                {
                    if(Height(T->right->left) > Height(T->right->right))
                    {
                        T = RL(T);
                    }
                    else T = RR(T);
                }
            }
            else if(v > T->value)
            {
                T->right = __DeleteNode(T->right,k,v);
                if(Height(T->left) - Height(T->right) == 2)
                {
                    if(Height(T->left->left) > Height(T->left->right))
                    {
                        T = LL(T);
                    }
                    else T = LR(T);
                }
            }
            else if(T->value == v){
                if(T->left && T->right)
                {
                    if(Height(T->left) > Height(T->right))
                    {
                        AVLTreeNode *max = T->left;
                        AVLTreeNode *temp = T;
                        int flag = 1;
                        while(max->right)
                        {
                            if(flag)
                            {
                                flag = 0;
                                temp = temp->left;
                            }
                            else temp =temp->right;
                            max = max->right;
                        }
                        T->key = max->key;
                        if(flag) temp->left = NULL;
                        else temp->right = NULL;
                        free(max);
                    }
                    else
                    {
                        AVLTreeNode *max = T->right;
                        AVLTreeNode *temp = T;
                        int flag = 1;
                        while(max->left)
                        {
                            if(flag)
                            {
                                flag = 0;
                                temp = temp->right;
                            }
                            else temp = temp->left;
                            max = max->left;
                        }
                        T->key = max->key;
                        if(flag) temp->right = NULL;
                        else temp->left = NULL;
                        free(max);
                    }
                }
                else
                {
                    AVLTreeNode *root = T;
                    T = (T->left ? T->left : T->right);
                    free(root);
                }
            }
        }
        return T;
    }
    
}
// put your time complexity for DeleteNode() here
// time complexity = O(logn)
int DeleteNode(AVLTree *T, int k, int v)
{
    // put your code here
		flag = 1;
    T->root = __DeleteNode(T->root,k,v);
    //T->size--;
		if (flag == 0) return 0;
    else {
			T->size--;
			return 1;
		}
}
AVLTreeNode *SearchNode(AVLTreeNode *T, int k, int v){
    
    if (T == NULL)
        return NULL;
    //if ((T->key==k)&&(T->value==v)){
    //    return T;}
    if (T->key < k){
        return SearchNode(T->left,k,v);
    }
    else if (T->key > k){
        return SearchNode(T->right,k,v);
    }
    else if (T->key == k){
        if (T->value < v){
            return SearchNode(T->left,k,v);
        }
        else if (T->key > v){
            return SearchNode(T->right,k,v);
        }
    }
		return T;
}
// put your time complexity analysis for Search() here
//// time complexity = O(logn)
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
    // put your code here
    if (T->size==0){
        return NULL;
    }
    return SearchNode(T->root,k,v);
}

void __FreeAVLTreeNode(AVLTreeNode *T){///postorder
    if (T != NULL){
        __FreeAVLTreeNode(T->left);
        __FreeAVLTreeNode(T->right);
        free(T);
    }
}

// put your time complexity analysis for freeAVLTree() here
//// time complexity = O(n)
void FreeAVLTree(AVLTree *T)
{
    // put your code here
    if (T != NULL){
        __FreeAVLTreeNode(T->root);
    }
    
}

void __PrintAVLTreeNode(AVLTreeNode *T)///inorder
{
        if (T != NULL){
            __PrintAVLTreeNode(T->left);
            printf("(%d, %d), %d\n",T->key,T->value,T->height);
            __PrintAVLTreeNode(T->right);
        }
}
    
// put your time complexity analysis for PrintAVLTree() here
// time complexity = O(n)
void PrintAVLTree(AVLTree *T)
{
        // put your code here
        if (T != NULL){
            __PrintAVLTreeNode(T->root);
        }
        
}

    int main() //sample main for testing
    { int i,j;
        AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
        AVLTreeNode *node1;
        /*
        tree1=CreateAVLTree("stdin");
        PrintAVLTree(tree1);
        FreeAVLTree(tree1);
        //you need to create the text file file1.txt
        // to store a set of items without duplicate items
				
        tree2=CreateAVLTree("file1.txt");
        PrintAVLTree(tree2);
        tree3=CloneAVLTree(tree2);
        PrintAVLTree(tree3);
        FreeAVLTree(tree2);
        FreeAVLTree(tree3);
			
        //Create tree4
        tree4=newAVLTree();
        j=InsertNode(tree4, 10, 10);
        for (i=0; i<15; i++)
        {
            j=InsertNode(tree4, i, i);
            if (j==0) printf("(%d, %d) already exists\n", i, i);
        }
        ///PrintAVLTree(tree4);
        node1=Search(tree4,20,20);
        if (node1!=NULL)
            printf("key= %d value= %d\n",node1->key,node1->value);
        else
            printf("Key 20 does not exist\n");
        
        for (i=17; i>0; i--)
        {
            j=DeleteNode(tree4, i, i);
            if (j==0)
                printf("Key %d does not exist\n",i);
            PrintAVLTree(tree4);
        }
        FreeAVLTree(tree4);
				*/
				
        //Create tree5
        tree5=newAVLTree();
        j=InsertNode(tree5, 6, 25);
        j=InsertNode(tree5, 6, 10);
        j=InsertNode(tree5, 6, 12);
        j=InsertNode(tree5, 6, 20);
        j=InsertNode(tree5, 9, 25);
        j=InsertNode(tree5, 10, 25);
				printf("a\n");
        PrintAVLTree(tree5);
				printf("b\n");
        //Create tree6
        tree6=newAVLTree();
        j=InsertNode(tree6, 6, 25);
        j=InsertNode(tree6, 5, 10);
        j=InsertNode(tree6, 6, 12);
        j=InsertNode(tree6, 6, 20);
        j=InsertNode(tree6, 8, 35);
        j=InsertNode(tree6, 10, 25);
        PrintAVLTree(tree6);
				printf("c\n");
        tree7=AVLTreesIntersection(tree5, tree6);
        tree8=AVLTreesUnion(tree5,tree6);
        PrintAVLTree(tree7);
        PrintAVLTree(tree8);
        return 0;
    }
