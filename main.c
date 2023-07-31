/*
 * API project 2022-2023.
 * All the algorithm and data structures were obtained from the book "Introduction to algorithm", Cormen-Leiserson-Rivest-Stein.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CARS_CAPACITY 128  // Default array maximum capacity to allocate.
#define ADD_STATION 1
#define REMOVE_STATION 2
#define ADD_CAR 3
#define REMOVE_CAR 4
#define PLAN_ROUTE 5
#define INF 2147483647  // Infinite value (maximum value of the integer).

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

/*
 * List that represent a node in the graph.
 */
typedef struct List{
    int num;  // Number of the node (= distance of the station).
    int d;  // Distance from the starting node, according to Dijkstra.
    int maxAut;  // Max autonomy of the station.
    struct List *pi;  // Previous node of the minimum path, according to Dijkstra.
    struct List *next;  // Next node of the graph, in topological order (increasing or decreasing order based on the algorithm).
}List;

// ---------------------------------------------------------------
// Binary tree.
// ---------------------------------------------------------------

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
void bstTransplant(NodeBst **root, NodeBst *u, NodeBst *v){
    if(u->p==NULL)
        (*root) = v;
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
void bstDelete(NodeBst **root, NodeBst *z){
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
    newStation->l = NULL;
    newStation->r = NULL;
    newStation->p = NULL;
    bstInsert(root, newStation);
    return newStation;
}

/*
 * Remove the selected station.
 * Return false if the station doesn't exist.
 */
bool removeStation(NodeBst **root, int distance){
    NodeBst *nodeToDelete = bstSearch(*root, distance);
    // If the station is not in the tree, do nothing.
    if(nodeToDelete == NULL)
        return false;
    // If the station is in the tree, remove it.
    bstDelete(root, nodeToDelete);
    return true;
}

/*
 * Add a car in the service station.
 * HP: the station exists in the tree.
 */
void addCar(NodeBst *station, int autonomy){
    Cars *cars = station->cars;
    // Check if the size is greater than the max capacity. If true allocate more memory.
    if(cars->size >= cars->maxCapacity){
        cars->maxCapacity += CARS_CAPACITY;
        cars->autonomy = realloc(cars->autonomy, cars->maxCapacity * sizeof(int));
    }
    // Increase the size of the list before adding the element.
    cars->size++;

    // If the current autonomy is greater than the saved one, substitute.
    if(autonomy > cars->maxAutonomy){
        cars->maxAutonomy = autonomy;
    }

    // Add the autonomy in the station.
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

    // Replace the maximum with 0 because the maximum value must be substituted.
    if(replaceMax)
        cars->maxAutonomy = 0;

    // Iterate all the cars.
    for(int i=0; i<cars->size; i++){
        // Remove the selected car if not already found. Different cars with the same value can exist; only one car must be deleted.
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

/*
 * Plan a route when the starting point is smaller than the ending point.
 * Print also the stations.
 */
void planRouteForward(NodeBst *root, int start, int end){
    int maxDist = 0;  // Max distance reachable from station x.
    List *headSortedNodes = NULL;  // HEAD of sorted nodes.
    List *tHead = NULL;  // Actual station to compare to the others.
    List *tSeek = NULL;  // Pointer to other stations to compare with tHead.
    NodeBst *bstNode = bstSearch(root, start);  // Node of the tree to be added to the graph.

    /*
     * Add sequentially from the starting point to the ending point all the possible intermediate nodes.
     * These are used by Dijkstra to calculate the minimum path from start to end.
     */
    // Add the first node separately. The starting node must have d=0.
    List *newNode = (List*)malloc(sizeof (List));
    newNode->num = bstNode->distance;
    newNode->d = 0;  // Distance from the starting node (itself) is 0, according to Dijkstra.
    newNode->maxAut = bstNode->cars->maxAutonomy;
    newNode->pi = NULL;
    headSortedNodes = newNode;
    tHead = headSortedNodes;
    tSeek = tHead;

    // List of all nodes from start to end.
    bstNode = bstSuccessor(bstNode);
    while(bstNode!=NULL && bstNode->distance <= end) {
        newNode = (List *) malloc(sizeof(List));
        newNode->num = bstNode->distance;
        newNode->d = INF;  // Distance from the starting node to INF according to Dijkstra.
        newNode->maxAut = bstNode->cars->maxAutonomy;
        tHead->next = newNode;
        tHead = tHead->next;
        bstNode = bstSuccessor(bstNode);
    }

    // Close the list. Bring tHead to the starting point.
    tHead->next = NULL;
    tHead = headSortedNodes;
    tSeek = tHead;

    /*
     * Iterate all over the nodes to find the minimum path (Dijkstra).
     * If a node has the same distance from the starting node, take the one with minimum value (from specs).
     */
    while(tHead->num != end){  // As long as tHead is different from the final node, iterate.
        maxDist = tHead->num + tHead->maxAut;  // Maximum distance the car can travel from the current station.
        tSeek = tHead->next;  // Successor of the current node.
        while(tSeek!=NULL && maxDist >= tSeek->num){  // Until the maximum car covers the stations, add them to the list.
            if(tSeek->d > tHead->d + 1){  // Relaxing if there is a path with fewer nodes.
                tSeek->d = tHead->d + 1;
                tSeek->pi = tHead;  // Mark tHead as previous node.
            } else if(tSeek->d == tHead->d + 1){  // If there is a path with the same nodes, evaluate the minor and save that as previous.
                if(tSeek->pi->d > tHead->d){
                    tSeek->pi = tHead;
                }
            }
            tSeek = tSeek->next;
        }
        tHead = tHead->next;

        if(tHead->d == INF){  // If I am in a node that has never been visited, it has no incoming edges. Consequently it means it's not reachable
            printf("nessun percorso\n");
            return;
        }
        if(tHead->num == end)  // End node reached.
            break;
    }
    // Save all the values from the end to the start.
    int len = tHead->d + 1;
    int finalSequence[len];
    int i = len-1;
    // I retrace all paths from end to start and save them in an array, end to start.
    while(tHead != NULL){
        finalSequence[i] = tHead->num;
        i--;
        tHead = tHead->pi;
    }
    // Print the values in the right order.
    for(i=0; i<len; i++)
        printf("%d ", finalSequence[i]);
    printf("\n");
}

/*
 * Plan a route when the starting point is greater than the ending point.
 */
void planRouteBackward(NodeBst *root, int start, int end){
    int maxDist = 0;  // Max distance reachable from station x.
    List *headSortedNodes = NULL;  // HEAD of sorted nodes.
    List *tHead = NULL;  // Actual station to compare to the others.
    List *tSeek = NULL;  // Pointer to other stations to compare with tHead.
    NodeBst *bstNode = bstSearch(root, start);  // Node of the tree to be added to the graph.

    /*
     * Add sequentially from the starting point to the ending point all the possible intermediate nodes.
     * These are used by Dijkstra to calculate the minimum path from start to end.
     */
    // Add the first node separately. The starting node must have d=0.
    List *newNode = (List*)malloc(sizeof (List));
    newNode->num = bstNode->distance;
    newNode->d = 0;  // Distance from the starting node (itself) is 0 according to Dijkstra.
    newNode->maxAut = bstNode->cars->maxAutonomy;
    newNode->pi = NULL;
    headSortedNodes = newNode;
    tHead = headSortedNodes;
    tSeek = tHead;

    // List of all nodes from start to end.
    bstNode = bstPredecessor(bstNode);
    while(bstNode!=NULL && bstNode->distance >= end){
        newNode = (List*)malloc(sizeof (List));
        newNode->num = bstNode->distance;
        newNode->d = INF;  // Distance from the starting node to INF according to Dijkstra.
        newNode->maxAut = bstNode->cars->maxAutonomy;
        tHead->next = newNode;
        tHead = tHead->next;
        bstNode = bstPredecessor(bstNode);
    }

    // Close the list. Bring tHead to the starting point.
    tHead->next = NULL;
    tHead = headSortedNodes;
    tSeek = tHead;

    /*
     * Iterate all over the nodes to find the minimum path (Dijkstra).
     * If a node has the same distance from the starting node, take the one with minimum value (from specs).
     */
    while(tHead->num != end){  // As long as tHead is different from the final node, iterate.
        maxDist = tHead->num - tHead->maxAut;  // Maximum distance the car can travel from the current station.
        tSeek = tHead->next;  // Successor of the current node.
        while(tSeek!=NULL && maxDist <= tSeek->num){  // Until the maximum car covers the stations, add them to the list.
            if(tSeek->d > tHead->d + 1){  // Relaxing if there is a path with fewer nodes.
                tSeek->d = tHead->d + 1;
                tSeek->pi = tHead;  // Mark tHead as previous node.
            } else if(tSeek->d == tHead->d + 1){  // If there is a path with the same nodes, evaluate the minor and save that as previous.
                if(tSeek->pi->num > tHead->num){
                    tSeek->pi = tHead;
                }
            }
            tSeek = tSeek->next;
        }
        tHead = tHead->next;

        if(tHead->d == INF){  // If I am in a node that has never been visited, it has no incoming edges. Consequently it means it's not reachable
            printf("nessun percorso\n");
            return;
        }
        if(tHead->num == end)  // End node reached.
            break;
    }
    // Save all the values from the end to the start.
    int len = tHead->d + 1;
    int finalSequence[len];
    int i = len-1;
    // I retrace all paths from end to start and save them in an array, end to start.
    while(tHead != NULL){
        finalSequence[i] = tHead->num;
        i--;
        tHead = tHead->pi;
    }
    // Print the values in the right order.
    for(i=0; i<len; i++)
        printf("%d ", finalSequence[i]);
    printf("\n");
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
        integer += (*ch - '0');  // Convert the char to an integer and subtract the char value corresponding to 0.
        *ch = getc_unlocked(stdin);
        if(*ch == '\n' || *ch == ' ' || *ch == -1){  // If \n, space or EOF is found, forward!
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

    *in = getc_unlocked(stdin);
    // Loop until EOF reached.
    while(*in != -1){
        // Set the status based on the written command.
        switch(*in){
            case 'a':
                for(int i=1; i<=9; i++)  // Skip all unuseful chars. Bring "in" on the first char after the command and the space.
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
                    printf("non aggiunta\n");
                    break;
                }
                for(int i=1; i<=carNumber; i++){
                    addCar(newStation, stringToInt(in));
                }
                printf("aggiunta\n");
                break;
            }

            case ADD_CAR:{
                int distance = stringToInt(in);
                int autonomy = stringToInt(in);
                NodeBst *station = bstSearch(rootStations, distance);
                if(station == NULL || autonomy < 0){  // If null it doesn't exist in the tree.
                    printf("non aggiunta\n");
                    break;
                }
                addCar(station, autonomy);
                printf("aggiunta\n");
                break;
            }

            case REMOVE_STATION:{
                int distance = stringToInt(in);
                bool removed = removeStation(&rootStations, distance);
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
                int distanceStart = stringToInt(in);  // The stations are certainly in the tree (from specs).
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
    return 0;
}
