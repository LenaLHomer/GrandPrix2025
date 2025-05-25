 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "listProjet.h"

List newList() {
    return NULL;
}

List addNodeInList(Node* node, List L) {
    List newCell;
    
    if (L == NULL) {
        return node;
    }
    newCell = node;
    newCell->nextCell = L;
    
    return newCell;
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
