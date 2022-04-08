#include <stddef.h>

#include "flist.h"
  
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const OFFSET = 2;

void file_init(struct file *m){
    for(int i = 0; i < FILE_SIZE; i++){
        m -> content[i] = NULL;
    }
    m -> size = 0;
}

key_t file_insert(struct file* m, value_t v){
    if(m -> size >= FILE_SIZE){
        return -1;
    }

    for(int i = 0; i < FILE_SIZE; i++){
        if(m->content[i] == NULL){
            m->content[i] = v;
            m->size ++;
            return i + OFFSET;
        }
    }
}

value_t file_find(struct file* m, key_t k){
    key_t index = k - OFFSET;
    if(m -> content[index] == NULL){
        return NULL;
    }
    
    return m -> content[index];
}

value_t file_remove(struct file* m, key_t k){
    value_t result = file_find(m, k);
    if(result == NULL){
        return NULL;
    }
    m->content[k] = NULL;
    m->size--;

    return result;
}

void file_for_each(struct file* m,
    void (*exec)(key_t k, value_t v, int aux),
    int aux){
        for(int i = 0; i < FILE_SIZE; i++){
            value_t temp = file_find(m, i);
            if( temp != NULL){
                exec(i, temp, aux);
            }
        }
    }

void file_remove_if(struct file* m,
    bool (*cond)(key_t k, value_t v, int aux),
    int aux){
        for(int i = 0; i < FILE_SIZE; i++){
            value_t temp = file_find(m, i);
            bool condition = cond(i, temp, aux);
            if( condition){
                file_remove(m, i);
            }
        }
    }
