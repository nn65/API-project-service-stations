#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CAPACITY 50  // Default array size to allocate.
#define ADD_STATION 1
#define REMOVE_STATION 2
#define ADD_CAR 3
#define REMOVE_CAR 4
#define PLAN_ROUTE 5

/*
 * Node of the binary search tree.
 */
struct NodeBst{
    unsigned int distance;  // Distance from the start.
    struct Cars *cars;  // List of cars in the station.
    struct NodeBst *l;  // Left node.
    struct NodeBst *r;  // Right node.
    struct NodeBst *p;  // Parent node.
};

/*
 * Heap data structure.
 */
struct Cars{
    int *autonomy;  // List of cars' autonomy.
    int size;  // Actual size of the list.
    int maxCapacity; // Maximum designed capacity of the list.
    int maxAutonomy;
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

/*
 * Binary search tree utility function to delete a node
 */
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

///*
// * Heap max heapify function.
// */
//void maxHeapify(struct Heap *h, int i){
//    unsigned int *a = h->autonomy;
//    int l = 2*i;
//    int r = 2*i+1;
//    int max;
//
//    if(l <= h->size && a[l] > a[i])
//        max = l;
//    else
//        max = i;
//    if(r <= h->size && a[r] > a[max])
//        max = r;
//    if(max != i){
//        unsigned int temp = h->autonomy[max];
//        h->autonomy[max] = h->autonomy[i];
//        h->autonomy[i] = temp;
//        maxHeapify(h, max);
//    }
//}
//
///*
// * Build an empty max heap.
// */
//struct Heap *buildMaxHeap(){
//    struct Heap* h = (struct Heap*)malloc(sizeof(struct Heap));
//    // Set size to 0. No element inside the heap.
//    // Set the initial capacity to the default value.
//    h->size = 0;
//    h->maxCapacity = CAPACITY;
//    h->autonomy = (unsigned int*)malloc(h->maxCapacity * sizeof(int));
//    return h;
//}
//
///*
// * Insert an element into the max heap.
// */
//void maxHeapInsert(struct Heap *h, int key){
//    // Check if the size is greater than the max capacity. If true allocate more memory.
//    if(h->size >= h->maxCapacity-1){
//        unsigned int *newAutonomy = realloc(h->autonomy, (h->maxCapacity+CAPACITY)*sizeof(int));
//        h->autonomy = newAutonomy;
//        h->maxCapacity += CAPACITY;
//    }
//
//    // If not the first item increase the size (= last element).
//    if(h->size != 0)
//        h->size++;
//    h->autonomy[h->size] = key;
//
//    unsigned int i = h->size;
//    while(i>0 && h->autonomy[i/2] < h->autonomy[i]){  // i/2 is the parent.
//        unsigned int temp = h->autonomy[i/2];
//        h->autonomy[i/2] = h->autonomy[i];
//        h->autonomy[i] = temp;
//        i = i/2;
//    }
//}
//
///*
// * Delete an element from the max heap.
// */
//void maxHeapDelete(struct Heap *h, int key){
//
//}

struct NodeBst *addStation(struct NodeBst *root, int distance){
    // If the station is in the tree, do nothing.
    if(bstSearch(root, distance) != NULL)
        return NULL;
    // If the station is not in the tree, add to it.
    struct NodeBst *newStation = (struct NodeBst*)malloc(sizeof(struct NodeBst));
    newStation->distance = distance;
    newStation->cars = (struct Cars*)malloc(sizeof(struct Cars));
    newStation->cars->autonomy = (int*)malloc(CAPACITY*sizeof(int));
    newStation->cars->size = 0;
    newStation->cars->maxAutonomy = 0;
    newStation->cars->maxCapacity = CAPACITY;
    bstInsert(root, newStation);
    return newStation;
}

bool removeStation(struct NodeBst *root, int distance){
    struct NodeBst *nodeToDelete = bstSearch(root, distance);
    // If the station is not in the tree, do nothing.
    if(nodeToDelete == NULL)
        return false;
    // If the station is in the tree, remove it.
    bstDelete(root, nodeToDelete);
    return true;
}

/*
 * Add a car in the service station.
 */
void addCar(struct NodeBst *station, int autonomy){
    struct Cars *cars = station->cars;
    // Check if the size is greater than the max capacity. If true allocate more memory.
    if(cars->size >= cars->maxCapacity){
        cars->maxCapacity += CAPACITY;
        cars->autonomy = realloc(cars->autonomy, cars->maxCapacity * sizeof(int));
    }

    cars->size++;

    // If the current autonomy is greater than the saved one, substitute.
    if(autonomy > cars->maxAutonomy){
        cars->maxAutonomy = autonomy;
    }

    cars->autonomy[cars->size-1] = autonomy;
}

/*
 * Remove a car from the service station.
 */
void removeCar(struct NodeBst *station, int autonomy){
    struct Cars *cars = station->cars;
    bool found = false;
    bool replaceMax = autonomy == cars->maxAutonomy;
    int next = 0;

    for(int i=0; i<cars->size; i++){
        // Remove the selected car if not already found
        if(!found){
            if(cars->autonomy[i] == autonomy){
                found = true;
            }
        }
        // If found start to shift all the positions to left, to replace the hole in the array.
        if(found){
            next = i+1;
            if(next > cars->size-1)  // carSize-1 is the last index of the array
                break;
            cars->autonomy[i] = cars->autonomy[next];
        }
        // Check the new maximum value
        if(replaceMax){
            if(cars->autonomy[i] > cars->maxAutonomy)
                cars->maxAutonomy = cars->autonomy[i];
        }
    }

    cars->size--;
}

/*
 * String to integer.
 */
int stringToInt(char ch){
    int integer = 0;
    while(1){
        integer += (ch - '0');  //Converto char in un int, togliendo lo zero di offset.
        ch = getc_unlocked(stdin);
        if(ch == '\n' || ch == ' '){
            ch = getc_unlocked(stdin);
            break;
        }
        integer *= 10;
        ch = getc_unlocked(stdin);
    }
    return integer;
}

int main() {
    int status = 0;
    struct NodeBst *rootStations = (struct NodeBst*)malloc(sizeof(struct NodeBst));
    // -------------------------------------------------
    // Input read.
    // -------------------------------------------------
    // Set the status.
    char in = getc_unlocked(stdin);
    switch(in){
        case 'a':
            for(int i=0; i<=10; i++)
                in = getc_unlocked(stdin);
            if(in == 's'){
                for(int i=0; i<=9; i++)
                    in = getc_unlocked(stdin);
                status = ADD_STATION;
            } else {
                for(int i=0; i<=5; i++)
                    in = getc_unlocked(stdin);
                status = ADD_CAR;
            }
            break;

        case 'd':
            for(int i=0; i<=19; i++)
                in = getc_unlocked(stdin);
            status = REMOVE_STATION;
            break;

        case 'r':
            for(int i=0; i<=13; i++)
                in = getc_unlocked(stdin);
            status = REMOVE_CAR;
            break;

        case 'p':
            for(int i=0; i<=19; i++)
                in = getc_unlocked(stdin);
            status = PLAN_ROUTE;
            break;

        default:
            status = 0;
    }

    switch(status){
        case ADD_STATION:{
            int distance = stringToInt(in);
            int carNumber = stringToInt(in);
            struct NodeBst *newStation = addStation(rootStations, distance);
            if(newStation == NULL){  // If null is already in the tree. Do nothing.
                printf("non aggiunta");
                break;
            }
            for(int i=1; i<=carNumber; i++){
                addCar(newStation, stringToInt(in));
            }
            printf("aggiunta");
            break;
        }

        case ADD_CAR:{
            int distance = stringToInt(in);
            int autonomy = stringToInt(in);
            struct NodeBst *station = bstSearch(rootStations, distance);
            if(station == NULL){  // If null it doesn't exist in the tree.
                printf("non aggiunta");
                break;
            }
            addCar(station, autonomy);
            printf("aggiunta");
            break;
        }

        case REMOVE_STATION:{
            int distance = stringToInt(in);
            bool removed = removeStation(rootStations, distance);
            if(removed)
                printf("demolita");
            else
                printf("non demolita");
        }

        case REMOVE_CAR:{
            
        }

        case PLAN_ROUTE:{
            printf("rotta pianificata o panificata")
        }

        default:
            printf("diocane");
    }

    return 0;
}
