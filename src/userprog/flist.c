#include <stddef.h>

#include "flist.h"
  
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <filesys/file.h>


const OFFSET = 2;


void file_table_init(struct file_table *m){
    for(int i = 0; i < FILE_TABLE_SIZE; i++){
        m -> content[i] = NULL;
    }
    m -> size = 0;
}

key_t file_table_insert(struct file_table* m, value_t v){
    if(m -> size >= FILE_TABLE_SIZE){
        return -1;
    }

    for(int i = 0; i < FILE_TABLE_SIZE; i++){
        if(m->content[i] == NULL){
            m->content[i] = v;
            m->size ++;
            return i + OFFSET;
        }
    }
}

value_t file_table_find(struct file_table* m, key_t k){
    key_t index = k - OFFSET;
    if(m -> content[index] == NULL){
        return NULL;
    }
    
    return m -> content[index];
}

value_t file_table_remove(struct file_table* m, key_t k){
    value_t result = file_table_find(m, k);
    if(result == NULL){
        return NULL;
    }
    m->content[k] = NULL;
    m->size--;

    return result;
}

void file_table_for_each(struct file_table* m,
    void (*exec)(key_t k, value_t v, int aux),
    int aux){
            for(int i = 0; i < FILE_TABLE_SIZE; i++){
                value_t temp = file_table_find(m, i);
                if( temp != NULL){
                    exec(i, temp, aux);
                }
            }
    }

void file_table_remove_if(struct file_table* m,
    bool (*cond)(key_t k, value_t v, int aux),
    int aux){
        for(int i = 0; i < FILE_TABLE_SIZE; i++){
            value_t temp = file_table_find(m, i);
            bool condition = cond(i, temp, aux);
            if( condition){
                file_table_remove(m, i);
            }
        }
    }

key_t file_table_find_file(struct file_table* m, value_t* f){
    for(int i = 0; i < FILE_TABLE_SIZE; i++){
        if(m->content[i] == f){
            return i + OFFSET;
        }
    }
    return -1;
}