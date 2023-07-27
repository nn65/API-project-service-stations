#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CARS_CAPACITY 50  // Default array maximum capacity to allocate.
#define ADD_STATION 1
#define REMOVE_STATION 2
#define ADD_CAR 3
#define REMOVE_CAR 4
#define PLAN_ROUTE 5
#define INF 64000;

/*
 * Node of the binary search tree.
 */
typedef struct NodeBst{
    int distance;  // Distance from the start.
    struct Cars *cars;  // List of cars in the station.
    struct NodeBst *l;  // Left node.
    struct NodeBst *r;  // Right node.
    struct NodeBst *p;  // Parent node.
}NodeBst;

/*
 * Cars data structure.
 */
typedef struct Cars{
    int *autonomy;  // List of cars' autonomy.
    int size;  // Actual size of the list.
    int maxCapacity; // Maximum designed capacity of the list.
    int maxAutonomy;  // Maximum autonomy in the list of cars.
}Cars;

typedef struct List{
    int num;
    int d;
    struct List *pi;
    struct List *next;
}List;

//typedef struct Edge{
//    struct NodeGraph *node;
//    struct Edge *next;
//}Edge;
//
//typedef struct NodeGraph{
//    int num;
//    int d;
//    struct Edge *edges;
//    struct NodeGraph *next;
//}NodeGraph;
//
//typedef struct List{
//    int data;
//    struct List *next;
//}List;
//
//typedef struct Graph{
//    struct List *pi;
//    struct NodeGraph *nodes;
//}Graph;

void printCars(NodeBst *station){
    for(int i=0; i<station->cars->size; i++){
        printf("%d ", station->cars->autonomy[i]);
    }
}

// ---------------------------------------------------------------
// Binary tree.
// ---------------------------------------------------------------

/*
 * Binary search tree in order tree walk.
 */
void bstInorderWalk(NodeBst *x){
    if(x!=NULL){
        bstInorderWalk(x->l);
        printf("Distanza: %d - ", x->distance);
        printf("Autonomie: ");
        printCars(x);
        printf("\n");
        bstInorderWalk(x->r);
    }
}

/*
 * Binary search tree search.
 */
NodeBst *bstSearch(NodeBst *x, int key){
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
void bstInsert(NodeBst **root, NodeBst *z){
    NodeBst *y, *x;
    y=NULL;
    x=(*root);
    while(x!=NULL){
        y=x;
        if(z->distance < x->distance)
            x = x->l;
        else
            x = x->r;
    }
    z->p = y;
    if(y==NULL)
        (*root) = z;  // The tree is empty.
    else if(z->distance < y->distance)
        y->l = z;
    else
        y->r = z;

}

/*
 * Binary search tree minimum key search.
 */
NodeBst *bstMin(NodeBst *x){
    while(x->l!=NULL)
        x=x->l;
    return x;
}

/*
 * Binary search tree maximum key search.
 */
NodeBst *bstMax(NodeBst *x){
    while(x->r!=NULL)
        x=x->r;
    return x;
}

/*
 * Binary search tree utility function to delete a node
 */
void bstTransplant(NodeBst *root, NodeBst *u, NodeBst *v){
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
 * Binary search tree node deletion.
 */
void bstDelete(NodeBst *root, NodeBst *z){
    NodeBst *y;
    if(z->l==NULL)
        bstTransplant(root, z, z->r);
    else if(z->r==NULL)
        bstTransplant(root, z, z->l);
    else {
        y = bstMin(z->r);
        if(y->p!=z){
            bstTransplant(root, y, y->r);
            y->r = z->r;
            y->r->p = y;
        }
        bstTransplant(root, z, y);
        y->l = z->l;
        y->l->p = y;
    }
}

/*
 * Binary search tree node successor.
 */
NodeBst *bstSuccessor(NodeBst *x){
    NodeBst *y;
    if(x->r != NULL)
        return bstMin(x->r);
    y = x->p;
    while(y!=NULL && x==y->r){
        x = y;
        y=y->p;
    }
    return y;
}

/*
 * Binary search tree node predecessor.
 */
NodeBst *bstPredecessor(NodeBst *x){
    NodeBst *y;
    if(x->l != NULL)
        return bstMax(x->l);
    y = x->p;
    while(y!=NULL && x==y->l){
        x = y;
        y=y->p;
    }
    return y;
}

// ---------------------------------------------------------------
// Graph.
// ---------------------------------------------------------------

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
//    h->maxCapacity = CARS_CAPACITY;
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
//        unsigned int *newAutonomy = realloc(h->autonomy, (h->maxCapacity+CARS_CAPACITY)*sizeof(int));
//        h->autonomy = newAutonomy;
//        h->maxCapacity += CARS_CAPACITY;
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


// ---------------------------------------------------------------
// Commands.
// ---------------------------------------------------------------

/*
 * Add a station to the tree.
 * Return the added station or null if it's already in the tree.
 */
NodeBst *addStation(NodeBst **root, int distance){
    // If the station is in the tree, do nothing.
    if(bstSearch(*root, distance) != NULL)
        return NULL;
    // If the station is not in the tree, add to it.
    NodeBst *newStation = (NodeBst*)malloc(sizeof(NodeBst));
    newStation->distance = distance;
    newStation->cars = (Cars*)malloc(sizeof(Cars));
    newStation->cars->autonomy = (int*)malloc(CARS_CAPACITY * sizeof(int));
    newStation->cars->size = 0;
    newStation->cars->maxAutonomy = 0;
    newStation->cars->maxCapacity = CARS_CAPACITY;
    bstInsert(root, newStation);
    return newStation;
}

/*
 * Remove the selected station.
 * Return false if the station doesn't exist.
 */
bool removeStation(NodeBst *root, int distance){
    NodeBst *nodeToDelete = bstSearch(root, distance);
    // If the station is not in the tree, do nothing.
    if(nodeToDelete == NULL)
        return false;
    // If the station is in the tree, remove it.
    bstDelete(root, nodeToDelete);
    return true;
}

/*
 * Add a car in the service station.
 * HP: the station exists in the tree and the autonomy is not 0.
 */
void addCar(NodeBst *station, int autonomy){
    Cars *cars = station->cars;
    // Check if the size is greater than the max capacity. If true allocate more memory.
    if(cars->size >= cars->maxCapacity){
        cars->maxCapacity += CARS_CAPACITY;
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
 * HP: the station exists in the tree.
 * Return false if the car doesn't exist in the station.
 */
bool removeCar(NodeBst *station, int autonomy){
    Cars *cars = station->cars;
    bool found = false;
    bool replaceMax = (autonomy == cars->maxAutonomy);
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

    // Only if the value is found, decrease the size.
    if(found)
        cars->size--;

    if(found)
        return true;
    else
        return false;
}

void planRouteForward(NodeBst *root, int start, int end){
    int maxDist = 0;  // Max distance reachable from station x.
    List *headSortedNodes = NULL;  // HEAD of sorted nodes.
    List *tHead = NULL;  // Actual station to compare to the others.
    List *tSeek = NULL;  // Last station added. Used to move between nodes.
    NodeBst *bstNode = bstSearch(root, start);  // Actual node to compare.

    List *newNode = (List*)malloc(sizeof (List));
    newNode->num = bstNode->distance;
    newNode->d = 0;
    headSortedNodes = newNode;
    tHead = headSortedNodes;
    tSeek = tHead;

    while(tHead->num != end){  // Fino a che il tHead è diverso dal nodo finale continua
        maxDist = bstNode->distance + bstNode->cars->maxAutonomy;  // Massima distanza che l'auto può raggiungere dalla stazione attuale
        bstNode = bstSuccessor(bstNode);  // Successore del nodo attuale
        while(bstNode!=NULL && maxDist >= bstNode->distance){  // Fino a che la massima distanza dell'auto copre le stazioni, aggiungile alla lista
            if(tSeek->next == NULL){  // Controllo se dopo ho un puntatore a NULL, aggiungo la stazione.
                newNode = (List*)malloc(sizeof (List));
                newNode->num = bstNode->distance;
                newNode->d = INF;
                tSeek->next = newNode;
            }
            tSeek = tSeek->next;
            if(tSeek->d > tHead->d + 1){  // Relaxing
                tSeek->d = tHead->d + 1;
                tSeek->pi = tHead;  // Segna il numero in tHead come precedente.
            }

            bstNode = bstSuccessor(bstNode);
        }
        tHead = tHead->next;
        tSeek = tHead;
        if(tHead == NULL) {  // Vuol dire che non esiste un successore
            printf("nessun percorso\n");
            return;
        }
        if(tHead->num == end)  // Sono arrivato al nodo di end.
            break;
        bstNode = bstSearch(root, tHead->num);
    }
    while(tHead != NULL){
        printf("%d ", tHead->num);
        tHead = tHead->pi;
    }
}

void planRouteSimpleForward(NodeBst *root, int start, int end){
    NodeBst *ref = bstSearch(root, start);
    NodeBst *seek = NULL;

    int maxDist = ref->distance + ref->cars->maxAutonomy;
    seek = bstSuccessor(ref);
    while(1){
        while(maxDist >= seek->distance){
            if(seek->distance + seek->cars->maxAutonomy > maxDist){
                ref = seek;
            }
            seek = bstSuccessor(seek);
        }
        maxDist = ref->distance + ref->cars->maxAutonomy;
        seek = bstSuccessor(ref);
    }

}

void planRouteSimpleBackward(NodeBst *root, int start, int end){

}

// ---------------------------------------------------------------
// Utilities.
// ---------------------------------------------------------------

/*
 * String to integer.
 */
int stringToInt(char *ch){
    int integer = 0;
    while(1){
        integer += (*ch - '0');  //Converto char in un int, togliendo lo zero di offset.
        *ch = getc_unlocked(stdin);
        if(*ch == '\n' || *ch == ' ' || *ch == -1){  // If \n is found, forward!
            *ch = getc_unlocked(stdin);
            break;
        }
        integer *= 10;
    }
    return integer;
}

// ---------------------------------------------------------------
// Main.
// ---------------------------------------------------------------

int main() {
    int status = 0;
    char *in = (char*)malloc(sizeof(char));
    NodeBst *rootStations = NULL;

    // -------------------------------------------------
    // Input read.
    // -------------------------------------------------
    *in = getc_unlocked(stdin);
    // Loop until EOF reached.
    while(*in != -1){
        // Set the status based on the written command.
        switch(*in){
            case 'a':
                for(int i=1; i<=9; i++)
                    *in = getc_unlocked(stdin);
                if(*in == 's'){
                    for(int i=1; i<=9; i++)
                        *in = getc_unlocked(stdin);
                    status = ADD_STATION;
                } else {
                    for(int i=1; i<=5; i++)
                        *in = getc_unlocked(stdin);
                    status = ADD_CAR;
                }
                break;

            case 'd':
                for(int i=1; i<=19; i++)
                    *in = getc_unlocked(stdin);
                status = REMOVE_STATION;
                break;

            case 'r':
                for(int i=1; i<=13; i++)
                    *in = getc_unlocked(stdin);
                status = REMOVE_CAR;
                break;

            case 'p':
                for(int i=1; i<=19; i++)
                    *in = getc_unlocked(stdin);
                status = PLAN_ROUTE;
                break;

            default:
                status = 0;
        }

        // Based on the command, do the right actions
        switch(status){
            case ADD_STATION:{
                int distance = stringToInt(in);
                int carNumber = stringToInt(in);
                NodeBst *newStation = addStation(&rootStations, distance);
                if(newStation == NULL){  // If null is already in the tree. Do nothing.
                    while((*in < 97 || *in > 122) && *in != -1)  // Forward the input until next command. (97=a, 122=z)
                        *in = getc_unlocked(stdin);
                    printf("non aggiunta\n");// %d\n", distance);
                    break;
                }
                for(int i=1; i<=carNumber; i++){
                    addCar(newStation, stringToInt(in));
                }
                printf("aggiunta\n");// %d\n", distance);
                break;
            }

            case ADD_CAR:{
                int distance = stringToInt(in);
                int autonomy = stringToInt(in);
                NodeBst *station = bstSearch(rootStations, distance);
                if(station == NULL || autonomy <= 0){  // If null it doesn't exist in the tree.
                    printf("non aggiunta\n");
                    break;
                }
                addCar(station, autonomy);
                printf("aggiunta\n");
                break;
            }

            case REMOVE_STATION:{
                int distance = stringToInt(in);
                bool removed = removeStation(rootStations, distance);
                if(removed)
                    printf("demolita\n");
                else
                    printf("non demolita\n");
                break;
            }

            case REMOVE_CAR:{
                int distance = stringToInt(in);
                int autonomy = stringToInt(in);
                bool removed = false;
                NodeBst *station = bstSearch(rootStations, distance);
                if(station == NULL){  // If null it doesn't exist in the tree.
                    removed = false;
                } else {
                    removed = removeCar(station, autonomy);
                }

                if(removed)
                    printf("rottamata\n");
                else
                    printf("non rottamata\n");
                break;
            }

            case PLAN_ROUTE:{
                int distanceStart = stringToInt(in);  // The stations are certainly in the tree (da specifica).
                int distanceEnd = stringToInt(in);

                if(distanceStart < distanceEnd)
                    planRouteForward(rootStations, distanceStart, distanceEnd);
                else
                    planRouteBackward(rootStations, distanceStart, distanceEnd);
                break;
            }

            default:
                return 0;
        }
    }

    //bstInorderWalk(rootStations);
    return 0;
}
