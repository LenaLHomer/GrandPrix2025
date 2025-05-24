#ifndef HEAP_H
#define HEAP_H


typedef struct sheap{
    int n; //maximal number of elements
    int nbElements; //number of elements in the heap
    int *position; //array of the positions of the elements in the heap
    int *heap; //array used to implement the heap
    double *priority; //priority of each element used to insert the elements in the heap
} Heap;


int getElement(Heap h);

int removeElement(Heap *h);

void modifyPriorityHeap(Heap *h, int element, double priority) ;

void freeHeap(Heap* h);

#endif