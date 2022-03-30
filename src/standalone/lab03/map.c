#include "map.h"
#include <stdlib.h>
//#include <stdbool.h>

void map_init(struct map* m){
    m -> next_pos = 0;
    m -> size = 0;
}

key_t map_insert(struct map* m, value_t v){
    if(m -> next_pos > MAP_SIZE){
        return NULL;
    }
    m -> content[m -> next_pos] = v;
    m -> next_pos ++;
    return m -> next_pos - 1;
}

value_t map_find(struct map* m, key_t k){
    // for(int i = 0; i < m -> size; i++){
    //     if(m -> content[i] == k)
    // }

    value_t test;
    return test;
}

value_t map_remove(struct map* m, key_t k){
    value_t test;
    return test;
}