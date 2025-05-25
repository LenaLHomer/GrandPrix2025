#ifndef ASTAR_H
#define ASTAR_H

double euristic(Node* a, Node* b);

double cost(Node* a, Node* b);

int isInList(List L, Node* n);

int isInHeap(Heap*h, int element);

void NodeinList(Node* node, List *L);

int AStar(Node* start, Node* goal, Node* nodeArray, int totalNodes, char** map, int width, int height);

#endif


