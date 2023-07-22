#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CAPACITY 20  //Dimensione alfabeto.

/*
 * Node of the binary search tree.
 */
struct NodeBst{
    unsigned int distance;  // Distance from the start
    struct Heap *carsAutonomy;  // List of cars' autonomy
    struct NodeBst *l;  // Left node
    struct NodeBst *r;  // Right node
    struct NodeBst *p;  // Parent node
};

/*
 * Heap data structure.
 */
struct Heap{
    unsigned int *autonomy;  // List of cars' autonomy
    unsigned int size;  // Actual size of the list
    unsigned int maxCapacity; // Maximum designed capacity of the list
};

/*
 * Binary search tree in order tree walk.
 */
void bstInorderWalk(struct NodeBst *x){
    if(x!=NULL){
        bstInorderWalk(x->l);
        //printf(x->distance);
        bstInorderWalk(x->r);
    }
}

/*
 * Binary search tree search.
 */
struct NodeBst *bstSearch(struct NodeBst *x, int key){
    while (x!=NULL && key!=x->distance){
        if(key < x->distance)
            x = x->l;
        else
            x = x->r;
    }
    return x;
}

/*
 * Binary search tree insert.
 */
void bstInsert(struct NodeBst *root, struct NodeBst *z){
    struct NodeBst *y, *x;
    y=NULL;
    x=root;
    while(x!=NULL){
        y=x;
        if(z->distance < x->distance)
            x = x->l;
        else
            x = x->r;
        z->p = y;
        if(y==NULL)
            root = z;  // The tree is empty.
        else if(z->distance < y->distance)
            y->l = z;
        else
            y->r = z;
    }
}

/*
 * Binary search tree minimum key search.
 */
struct NodeBst *bstMin(struct NodeBst *x){
    while(x->l!=NULL)
        x=x->l;
    return x;
}

/*
 * Binary search tree maximum key search.
 */
struct NodeBst *bstMax(struct NodeBst *x){
    while(x->r!=NULL)
        x=x->r;
    return x;
}

void bstTransplant(struct NodeBst *root, struct NodeBst *u, struct NodeBst *v){
    if(u->p==NULL)
        root = v;
    else if(u==u->p->l)
        u->p->l = v;
    else
        u->p->r = v;
    if(v!=NULL)
        v->p = u->p;
}

/*
 * Binary search tree node deletion. FORSE E' SBAGLIATO. CONTROLLARE.
 */
void bstDelete(struct NodeBst *root, struct NodeBst *z){
    struct NodeBst *y;
    if(z->l==NULL)
        bstTransplant(root, z, z->r);
    else if(z->r==NULL)
        bstTransplant(root, z, z->l);
    else
        y= bstMin(z->r);
    if(y->p!=z){
        bstTransplant(root, y, y->r);
        y->r = z->r;
        y->r->p = y;
    }
    bstTransplant(root, z, y);
    y->l = z->l;
    y->l->p = y;
}

/*
 * Heap max heapify function.
 */
void maxHeapify(struct Heap *h, int i){
    unsigned int *a = h->autonomy;
    int l = 2*i;
    int r = 2*i+1;
    int max;

    if(l <= h->size && a[l] > a[i])
        max = l;
    else
        max = i;
    if(r <= h->size && a[r] > a[max])
        max = r;
    if(max != i){
        unsigned int temp = h->autonomy[max];
        h->autonomy[max] = h->autonomy[i];
        h->autonomy[i] = temp;
        maxHeapify(h, max);
    }
}

struct Heap *buildMaxHeap(){
    struct Heap* h = (struct Heap*)malloc(sizeof(struct Heap));
    // Set size to 0. No element inside the heap.
    // Set the initial capacity to 10 (default value).
    h->size = 0;
    h->maxCapacity = CAPACITY;
    h->autonomy = (unsigned int*)malloc(h->maxCapacity * sizeof(int));
    return h;
}

void maxHeapInsert(struct Heap *h, int key){
    // Check if the size is greater than the max capacity. If true allocate more memory.
    if(h->size >= h->maxCapacity-1){
        unsigned int *newAutonomy = realloc(h->autonomy, (h->maxCapacity+CAPACITY)*sizeof(int));
        h->autonomy = newAutonomy;
        h->maxCapacity += CAPACITY;
    }

    if(h->size != 0)
        h->size++;
    h->autonomy[h->size] = key;

    int i = h->size;
    while(i>0 && h->autonomy[i/2] < h->autonomy[i]){
        unsigned int temp = h->autonomy[i/2];
        h->autonomy[i/2] = h->autonomy[i];
        h->autonomy[i] = temp;
        i = i/2;  // i/2 is the parent.
    }
}

int main() {
    char in = getc_unlocked(stdin);
    // --------------------------------------------------------
    // Aggiungo nodi all'albero
    // --------------------------------------------------------
    struct NodeBst *root_bst;

    return 0;
}
