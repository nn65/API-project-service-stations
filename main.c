#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Node of the binary search tree.
 */
struct NodeBst{
    unsigned int distance;  // Distance from the start
    struct NodeBst *l;  // Left node
    struct NodeBst *r;  // Right node
    struct NodeBst *p;  // Parent node
};

/*
 * Node of the heap.
 */
struct NodeHeap{
    unsigned int autonomy;  // Car autonomy.
    struct NodeHeap *l;  // Left node
    struct NodeHeap *r;  // Right node
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
 *
 */
void maxHeapify(struct NodeHeap *root, struct NodeHeap *z){
    struct NodeHeap *l, *r;
    l = z->l;
    r = z->r;

}

int main() {
    char in = getc_unlocked(stdin);
    // --------------------------------------------------------
    // Aggiungo nodi all'albero
    // --------------------------------------------------------
    struct NodeBst *root_bst;

    return 0;
}
