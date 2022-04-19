#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


#include "plist.h"

void plist_init(struct plist* pl){
    for(int i = 0; i < PLIST_SIZE; i++){
        pl -> content[i].is_in_list = false;
    }
    pl -> size = 0;
}

key_t plist_insert(struct plist* pl, value_t v){
    key_t index = -1;
    if(pl -> size >= PLIST_SIZE){
        return -1;
    }
    
    for(int i = 0; i < PLIST_SIZE; i++){
        if(!(pl->content[i].is_in_list)){
            pl->content[i] = v;
            pl->size ++;
            index = i;
            break;
        }
    }
    return index;
}

value_t plist_find(struct plist* pl, key_t k){

    if(!(pl -> content[k].is_in_list)){
        return;
    }
    return pl -> content[k];
}

value_t plist_remove(struct plist* pl, key_t k){
    //value_t result = plist_find(pl, k);
  
    if(!(pl->content[k].status_needed)) 
    {
        pl->content[k].is_in_list = false;
        pl->size--;
    }
    return;
}

void plist_print(struct plist* pl){
    for(int i = 0; i < pl->size; i++){
        printf("%10s%10s%10s%10s%10s%10s\n", "In table", "PID", "Alive",
         "Status", "Parent id", "Status needed");
        printf("%s\n", '='*30);
        printf("%10d | %10d | %10d | %10d | %10d | %10d\n", pl->content[i].is_in_list,
                                                            pl->content[i].my_pid,
                                                            pl->content[i].alive,
                                                            pl->content[i].status,
                                                            pl->content[i].parent_pid,
                                                            pl->content[i].status_needed);
        printf("%s\n", '='*30);

    }
}