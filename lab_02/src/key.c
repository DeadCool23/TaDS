#include "key.h"
#include "theatreio.h"
#include "helper.h"

#include <stdlib.h>

void init(key_table_t *key) {
    key->size = 0;
    key->keys = NULL;
    key->key_table_gen = key_table_gen;
    key->free_key_table = free_key_table;
}

void key_table_gen(key_table_t *key_table, theatre_table_t table) {
    key_table->keys = table.size ? malloc(sizeof(*(key_table->keys)) * table.size) : NULL;
    if (!key_table->keys && table.size) return;
    key_table->size = table.size;
    for (size_t i = 0; i < key_table->size; i++) {
        strcpy(key_table->keys[i].theatre_name, table.theatres[i].theatre_name);
        key_table->keys[i].ind = i;
    }
}

void free_key_table(key_table_t *key_table) { 
    free(key_table->keys);
    key_table->keys = NULL;
    key_table->size = 0;
}

void key_table_print(key_table_t *key_table, theatre_table_t *table) {
    header('t');
    for (size_t i = 0; i < key_table->size; i++)
        theatre_print(&table->theatres[key_table->keys[i].ind]);
    ending('t');
}

void keys_print(key_table_t *key_table) {
    header('k');
    for (size_t i = 0; i < key_table->size; i++)
        printf("|%5zu|%-30s|\n", key_table->keys[i].ind, key_table->keys[i].theatre_name);
    ending('k');
}

