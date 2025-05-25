#include <stdio.h>
#include <stdlib.h>
#include "HeapProjet.h"

void swap(Heap *h, int i, int j) {
    int temp = h->heap[i];
    h->heap[i] = h->heap[j];
    h->heap[j] = temp;

    temp = h->position[h->heap[i]];
    h->position[h->heap[i]] = h->position[h->heap[j]];
    h->position[h->heap[j]] = temp;
}

Heap* createHeap(int n) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->n = n;
    h->nbElements = 0;
    h->position = (int*)malloc((n) * sizeof(int));
    h->heap = (int*)malloc((n) * sizeof(int));
    h->priority = (double*)malloc((n) * sizeof(int));

    for(int i = 0; i < n; i++){
        h->position[i] = -1;
        h->heap[i] = -1;
        h->priority[i] = -1;
    }
    return h;
}

void insertHeap(Heap *h, int element, double priority) {
    int pos = h->nbElements;

    h->priority[element] = priority;
    h->position[element] = pos;
    h->heap[pos] = element;
    while(pos != 0){
        if(h->priority[h->heap[pos]] < h->priority[h->heap[pos-1]]){
            swap(h,pos-1,pos);
        }
        pos--;
    }


    h->nbElements++;
    return;
}

int getElement(Heap h) {
    double maximumPriority = __DBL_MAX__;
    int indice = -1;
    for(int i = 0; i < h.n; i++){
        if((h.priority[i] != -1) && (h.priority[i] < maximumPriority)){
            maximumPriority = h.priority[i];
            indice = i;
        }
    }
    return indice;
}

int removeElement(Heap *h) {
    int elem = h->heap[0];

    for(int i = 0; i < h->nbElements; i++){
        swap(h,i,i+1);
    }
    h->heap[h->nbElements-1] = -1;
    h->priority[elem] = -1;
    h->position[elem] = -1;

    return elem;
}

void modifyPriorityHeap(Heap *h, int element, double priority) {
    int pos = h->nbElements-1;
    h->priority[element] = priority;

    while(pos != 0){
        if(h->priority[h->heap[pos]] < h->priority[h->heap[pos-1]]){
            swap(h,pos-1,pos);
        }
        pos--;
    }

    while(pos != h->nbElements-1){
        if(h->priority[h->heap[pos+1]] < h->priority[h->heap[pos]]){
            swap(h,pos+1,pos);
        }
        pos++;
    }

    return;
}

void freeHeap(Heap* h) {
    free(h->heap);
    free(h);
}