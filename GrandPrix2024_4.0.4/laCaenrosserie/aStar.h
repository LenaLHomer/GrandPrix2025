#ifndef ASTAR_H
#define ASTAR_H

#include "listProjet.h"
#include "heapProjet.h"

double euristic(Node* a, Node* b);

double cost(Node* a, char** map);

int isInList(List L, Node* n);

int isInHeap(Heap* h, int element);

int aStar(Node* start, Node* goal, Node* nodeArray, int totalNodes, char** map, int width, int height);

#endif


