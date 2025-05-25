 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "listProjet.h"

List newList() {
    return NULL;
}

void nodeInList(Node* node, List *L) {
    List newCell;
    newCell = malloc(sizeof(List));
    if (newCell == NULL) {
        return;
    }
    newCell = &node;
    newCell->nextCell = L;
    L = newCell;
}


void freeList(List L) {
    List next;
    while(L != NULL){
        next = L->nextCell;
        free(L);
        L = next;
    }
    return;
}
