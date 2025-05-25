#ifndef LIST_H
#define LIST_H

#define MAX_NEIGHBORS 4

/* on en a bien besoin ? */
typedef struct Point{
    int x;
    int y;
} Point;

typedef struct Node {
    int x, y;
    int direction;
    double heuristic;
    double total_cost;
    double g, f, h;
    int id;
    struct Node* voisins[MAX_NEIGHBORS];
    int nbVoisins;
    struct Node *nextCell;
    struct Node *parent;
} Node, *List;


List newList();

List addNodeInList(Node* node, List L);

void freeList(List L);

#endif
