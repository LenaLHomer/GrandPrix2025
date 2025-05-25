#ifndef HEAP_H
#define HEAP_H


typedef struct sheap{
    int n;
    int nbElements;
    int *position;
    int *heap;
    double *priority;
} Heap;

void swap(Heap *h, int i, int j);

Heap* createHeap(int n);

void insertHeap(Heap *h, int element, double priority);

int getElement(Heap h);

int removeElement(Heap *h);

void modifyPriorityHeap(Heap *h, int element, double priority) ;

void freeHeap(Heap* h);

#endif