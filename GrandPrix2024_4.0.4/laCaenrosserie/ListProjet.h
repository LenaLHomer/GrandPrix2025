#ifndef LIST_H
#define LIST_H

typedef struct Node{
    int x;
    int y;
    int direction;
    double heuristic;
    double total_cost;
    struct Node *nextCell;
    struct Node *parent;
} Node, *List;

List newList();

void NodeinList(Node node, List L);

void freeList(List L);

#endif