/* do not forget the guard against multiple includes */

#pragma once

typedef char* value_t;
typedef int key_t;

#define MAP_SIZE 128

/* stores all objects in list or array */
struct map {
    value_t content[MAP_SIZE];
    int next_pos;
    int size;
};

void map_init(struct map* m);

key_t map_insert(struct map* m, value_t v);

value_t map_find(struct map* m, key_t k);

value_t map_remove(struct map* m, key_t k);