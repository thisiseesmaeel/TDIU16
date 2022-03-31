#include "map.h"
#include <stdlib.h>
#include <stdio.h>

//#include <stdbool.h>

void map_init(struct map *m){
    for(int i = 0; i < MAP_SIZE; i++){
        m -> content[i] = "";
    }
    m -> size = 0;
}

key_t map_insert(struct map* m, value_t v){
    if(m -> size >= MAP_SIZE){
        return NULL;
    }

    for(int i = 0; i < MAP_SIZE; i++){
        if(m->content[i] == ""){
            m->content[i] = v;
            m->size ++;
            return i;
        }
    }
}

value_t map_find(struct map* m, key_t k){
    if(m -> content[k] == ""){
        return NULL;
    }
    
    return m -> content[k];
}

value_t map_remove(struct map* m, key_t k){
    value_t test = map_find(m, k);

    //printf("%s %d\n", "test: ", test);
    if(test == NULL){
        return NULL;
    }

    m->content[k] = "";
    m->size--;

    return test;
}

