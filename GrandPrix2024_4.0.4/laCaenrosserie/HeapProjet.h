#ifndef HEAP_H
#define HEAP_H


typedef struct sheap{
    int n;
    int nbElements;
    int *position;
    int *heap;
    double *priority;
} Heap;


int getElement(Heap h);

int removeElement(Heap *h);

void modifyPriorityHeap(Heap *h, int element, double priority) ;

void freeHeap(Heap* h);

#endif