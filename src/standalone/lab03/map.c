#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void map_init(struct map *m){
    for(int i = 0; i < MAP_SIZE; i++){
        m -> content[i] = NULL;
    }
    m -> size = 0;
}

key_t map_insert(struct map* m, value_t v){
    if(m -> size >= MAP_SIZE){
        return -1;
    }

    for(int i = 0; i < MAP_SIZE; i++){
        if(m->content[i] == NULL){
            m->content[i] = v;
            m->size ++;
            return i;
        }
    }
}

value_t map_find(struct map* m, key_t k){
    if(m -> content[k] == NULL){
        return NULL;
    }
    
    return m -> content[k];
}

value_t map_remove(struct map* m, key_t k){
    value_t result = map_find(m, k);
    if(result == NULL){
        return NULL;
    }
    m->content[k] = NULL;
    m->size--;

    return result;
}

void map_for_each(struct map* m,
    void (*exec)(key_t k, value_t v, int aux),
    int aux){
        for(int i = 0; i < MAP_SIZE; i++){
            value_t temp = map_find(m, i);
            if( temp != NULL){
                exec(i, temp, aux);
            }
        }
    }

void map_remove_if(struct map* m,
    bool (*cond)(key_t k, value_t v, int aux),
    int aux){
        for(int i = 0; i < MAP_SIZE; i++){
            value_t temp = map_find(m, i);
            bool condition = cond(i, temp, aux);
            if( condition){
                map_remove(m, i);
            }
        }
    }
