#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heapProjet.h"
#include "listProjet.h"
#include "aStar.h"
#include "follow_line.h"

#define MAX_NEIGHBORS 4


double heuristic(Node* a, Node* b) {
    return cost(a, b)*((double)(abs(a->x - b->x) + abs(a->y - b->y)));
}


double cost(Node* a, Node* b) {
    /* à moduler selon le type de point */
    return 1.0;
}


int isInList(List L, Node* n) {
    while (L != NULL) {
        if (L == n) {
            return 1;
        }
        L = L->nextCell;
    }
    return 0;
}


int isInHeap(Heap* h, int element) {
    return h->position[element] != -1;
}


int lineOfSight(char** map, int width, int height, int x1, int y1, int x2, int y2) {
    InfoLine line;
    Pos2Dint point;

    initLine(x1, y1, x2, y2, &line);
    while (nextPoint(&line, &point, 1) > 0) {
        if (point.x < 0 || point.y < 0 || point.x >= width || point.y >= height || map[point.y][point.x] == '#') {
            return 0;
        }
    }
    return 1;
}

int aStar(Node* start, Node* goal, Node* nodeArray, int totalNodes, char** map, int width, int height) {
    int i = 0;
    Heap* open;
    List closed;
    int currentId;
    Node* current;
    Node* neighbor;
    double tentative_g;

    open = createHeap(totalNodes);
    closed = NULL;

    for (i = 0; i < totalNodes; i++) {
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
        currentId = removeElement(open);
        current = &nodeArray[currentId];

        if (current == goal) {
            freeHeap(open);
            freeList(closed);
            return 1;
        }

        closed = addNodeInList(current, closed);

        for (i = 0; i < current->nbVoisins; i++) {
            neighbor = current->voisins[i];
            if (!lineOfSight(map, width, height, current->x, current->y, neighbor->x, neighbor->y)) {
                continue;
            }
            tentative_g = current->g + cost(current, neighbor);

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
