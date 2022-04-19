#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


#include "plist.h"

void plist_init(struct plist* pl){
    for(int i = 0; i < PLIST_SIZE; i++){
        pl -> content[i] = NULL;
    }
    pl -> size = 0;
    printf("\nInitialized DONE\n");
}

key_t plist_insert(struct plist* pl, pl_value_t v){
    printf("\nEntering insert\n");
    key_t index = -1;
    printf("\n%d\n", pl->size);
    if((pl->size) >= PLIST_SIZE){
        printf("\n Inside first if\n");
        return -1;
    }
    

    for(int i = 0; i < PLIST_SIZE; i++){
        printf("\n%d\n", i);
        if((pl->content[i]) == NULL){
            pl->content[i] = v;
            pl->size ++;
            index = i;
            break;
        }
    }
    printf("\npassed2\n");
    return index;
}

pl_value_t plist_find(struct plist* pl, key_t k){

    if((pl -> content[k]) == NULL){
        return NULL;
    }
    return pl -> content[k];
}

pl_value_t plist_remove(struct plist* pl, key_t k){
    pl_value_t p = plist_find(pl, k); 
    if(p != NULL) 
    {
        pl->content[k] = NULL;
        pl->size--;
        return p;
    }
    return NULL;
}

void plist_print(struct plist* pl){
    for(int i = 0; i < pl->size; i++){
        printf("%10s%10s%10s%10s%10s\n", "PID", "Alive",
         "Status", "Parent id", "Status needed");
        printf("%s\n", (char *) ('='*30));
        printf("%10d | %10d | %10d | %10d | %10d\n", pl->content[i]->my_pid,
                                                            pl->content[i]->alive,
                                                            pl->content[i]->status,
                                                            pl->content[i]->parent_pid,
                                                            pl->content[i]->status_needed);
        printf("%s\n", (char *) ('='*30));
    }
}