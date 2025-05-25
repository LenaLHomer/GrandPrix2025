#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HeapProjet.h"
#include "ListProjet.h"

#define MAX_NEIGHBORS 4

typedef struct Node {
    int id;
    int x, y;
    double g, h, f;
    struct Node* parent;
    struct Node* voisins[MAX_NEIGHBORS];
    int nbVoisins;
    struct Node* nextCell;
} Node;

double heuristic(Node* a, Node* b) {
    return fabs(a->x - b->x) + fabs(a->y - b->y);
}


double cost(Node* a, Node* b) {
    return 1.0;
}


int isInList(List L, Node* n) {
    while (L != NULL) {
        if (L == n) return 1;
        L = L->nextCell;
    }
    return 0;
}


int isInHeap(Heap* h, int element) {
    return h->position[element] != -1;
}


void NodeinList(Node* node, List* L) {
    node->nextCell = *L;
    *L = node;
}


int AStar(Node* start, Node* goal, Node* nodeArray, int totalNodes) {
    Heap* open = createHeap(totalNodes);
    List closed = NULL;

    for (int i = 0; i < totalNodes; i++) {
        nodeArray[i].g = -1;
        nodeArray[i].f = -1;
        nodeArray[i].h = -1;
        nodeArray[i].parent = NULL;
        nodeArray[i].nextCell = NULL;
    }

    start->g = 0;
    start->h = heuristic(start, goal);
    start->f = start->h;
    insertHeap(open, start->id, start->f);

    while (open->nbElements > 0) {
        int currentId = removeElement(open);
        Node* current = &nodeArray[currentId];

        if (current == goal) {
            freeHeap(open);
            freeList(closed);
            return 1;
        }

        NodeinList(current, &closed);

        for (int i = 0; i < current->nbVoisins; i++) {
            Node* neighbor = current->voisins[i];
            double tentative_g = current->g + cost(current, neighbor);

            if (isInList(closed, neighbor)) continue;

            if (neighbor->g == -1 || tentative_g < neighbor->g) {
                neighbor->g = tentative_g;
                neighbor->h = heuristic(neighbor, goal);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                if (isInHeap(open, neighbor->id)) {
                    modifyPriorityHeap(open, neighbor->id, neighbor->f);
                } else {
                    insertHeap(open, neighbor->id, neighbor->f);
                }
            }
        }
    }

    freeHeap(open);
    freeList(closed);
    return 0;
}
