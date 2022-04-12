#include <stddef.h>

#include "flist.h"
  
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <filesys/file.h>
//#define UNUSED(x) (void)(x)


const int OFFSET = 2;

void file_table_init(struct file_table *ft){
    for(int i = 0; i < FILE_TABLE_SIZE; i++){
        ft -> content[i] = NULL;
    }
    ft -> size = 0;
}

key_t file_table_insert(struct file_table* ft, value_t v){
    if(ft -> size >= FILE_TABLE_SIZE){
        return -1;
    }

    int index = 0;
    for(int i = 0; i < FILE_TABLE_SIZE; i++){
        if(ft->content[i] == NULL){
            ft->content[i] = v;
            ft->size ++;
            index = i;
            break;
        }
    }
    return index + OFFSET;
}

value_t file_table_find(struct file_table* ft, key_t k){
    key_t index = k - OFFSET;

    if(ft -> content[index] == NULL){
        return NULL;
    }
    
    return ft -> content[index];
}

value_t file_table_remove(struct file_table* ft, key_t k){
    value_t result = file_table_find(ft, k);
    if(result == NULL){
        return NULL;
    }
    ft->content[k - OFFSET] = NULL;
    ft->size--;

    return result;
}

void file_table_for_each(struct file_table* ft,
                        void (*exec)(key_t k, value_t v, int aux),
                        int aux){
                            for(int i = 0; i < FILE_TABLE_SIZE; i++){
                                value_t temp = file_table_find(ft, i);
                                if( temp != NULL){
                                    (*exec)(i, temp, aux);
                                }
                            }
                        }

void file_table_remove_if(struct file_table* ft,
                        bool (*cond)(key_t k, value_t v, int aux),
                        int aux){
                            for(int i = 0; i < FILE_TABLE_SIZE; i++){
                                value_t temp = file_table_find(ft, i);
                                bool condition = cond(i, temp, aux);
                                if( condition ){
                                    file_table_remove(ft, i);
                                }
                            }
                        }

void file_table_close(struct file_table* ft){
  for(int i = 0; i < ft->size; i++){
      if(ft->content[i] != NULL){
        file_close(ft->content[i]);
        ft->content[i] = NULL;
      }
  }
}