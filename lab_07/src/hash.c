#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define _GNU_SOURCE

/// @brief Хэш-функция
static size_t hashing(string key, size_t size) {
    size_t index = 0;
    for (char *i = key; *i != '\0'; index += *i++);
    index = (index * strlen(key)) % size;
    return index;
}

// Close hash
//===========================================================================

void chash_init(chash_table_t *table, size_t size) {
    table->size = size;
    table->elements_count = 0;
    table->hash_func = hashing;
    table->hash_table = calloc(size, sizeof(*table->hash_table));
}

void chash_free(chash_table_t *table) {
    for (size_t i = 0; i < table->size; i++)
        if (table->hash_table[i])
            free(table->hash_table[i++]);
    free(table->hash_table);
    table->size = 0;
}

void crestructuring(chash_table_t *table, bool inf) {
    size_t new_size = table->size * 2;
    string *new_table = calloc(new_size, sizeof(string));
    chash_table_t cnew_table = {
        .size = new_size,
        .elements_count = 0,
        .hash_func = table->hash_func,
        .hash_table = new_table,
    };

    for (size_t i = 0; i < table->size; i++)
        if (table->hash_table[i])
            chash_add(&cnew_table, table->hash_table[i], inf);

    chash_free(table);
    *table = cnew_table;
}


void chash_add(chash_table_t *table, string key, bool inf) {
    size_t index = table->hash_func(key, table->size);

    bool is_full = true;
    if (!table->hash_table[index]) {
        table->hash_table[index] = strdup(key);
        is_full = false;
    } else {
        for (size_t i = (index + 1) % table->size, cnt = 0; cnt < MAX_COLLIZION_CNT && is_full; i = (i + 1) % table->size, cnt++) {
            if (!table->hash_table[i]) {
                table->hash_table[i] = strdup(key);
                is_full = false;
            }
        }
    }

    if (is_full) {
        if (inf) printf("Closed hash table was restructed\n");
        crestructuring(table, inf);
        chash_add(table, key, inf);
    } else
        table->elements_count++;
}

int chash_remove(chash_table_t *table, string key, size_t *cmp_cnt) {
    bool is_finded = false;
    size_t index = table->hash_func(key, table->size);
    if (cmp_cnt) (*cmp_cnt)++;
    if (!strcmp(table->hash_table[index] ? table->hash_table[index] : "\n", key)) {
        free(table->hash_table[index]);
        table->hash_table[index] = NULL;
        is_finded = true;
    } else {
        for (size_t i = (index + 1) % table->size, cnt = 0; cnt < MAX_COLLIZION_CNT && !is_finded; i = (i + 1) % table->size, cnt++) {
            if (cmp_cnt) (*cmp_cnt)++;
            if (!strcmp(table->hash_table[index] ? table->hash_table[index] : "\n", key)) {
                free(table->hash_table[i]);
                table->hash_table[i] = NULL;
                table->elements_count--;
                is_finded = true;
            }
        }
    }
    return !is_finded ? 0 : 1;
}

void chash_print(chash_table_t *table) {
    if (!table->elements_count) return;
    printf("Closed hash table:\n");
    printf("----+\n");
    for (size_t i = 0; i < table->size; i++)
        table->hash_table[i] ? printf( "%4zu| \033[0;33m%s\033[0m\n", i, table->hash_table[i])
                             : printf( "%4zu| %s\n", i, "empty");
    printf("----+\n");
}

// Open hash
//==========================================================================

void ohash_init(ohash_table_t *table, size_t size) {
    table->size = size;
    table->elements_count = 0;
    table->hash_func = hashing;
    table->hash_table = calloc(size, sizeof(*table->hash_table));
}

void orestructuring(ohash_table_t *table, bool inf) {
    size_t new_size = table->size * 2;
    hash_list_t **new_table = calloc(new_size, sizeof(*new_table));
    ohash_table_t onew_table = {
        .size = new_size,
        .elements_count = 0,
        .hash_func = hashing,
        .hash_table = new_table,
    };

    for (size_t i = 0; i < table->size; i++)
        for (hash_list_t *current = table->hash_table[i]; current; current = current->next)
            ohash_add(&onew_table, current->key, inf);

    ohash_free(table);
    *table = onew_table;
}

void ohash_add(ohash_table_t *table, string key, bool inf) {
    size_t index = table->hash_func(key, table->size);
    hash_list_t *new_node = malloc(sizeof(*new_node));
    new_node->index = 0;
    new_node->key = strdup(key);
    new_node->next = NULL;

    if (!table->hash_table[index]) {
        table->hash_table[index] = new_node;
    } else {
        new_node->index++;
        hash_list_t *current;
        for (current = table->hash_table[index]; current->next != NULL; current = current->next, new_node->index++);
        current->next = new_node;
    }

    if (new_node->index >= MAX_COLLIZION_CNT) {
        if (inf) printf("Opened hash table was restructed\n");
        orestructuring(table, inf);
    } else
        table->elements_count++;
}

int ohash_remove(ohash_table_t *table, string key, size_t *cmp_cnt) {
    bool is_finded = false;
    size_t index = table->hash_func(key, table->size);
    if (cmp_cnt) (*cmp_cnt)++;
    if (table->hash_table[index]) {
        if (!strcmp(table->hash_table[index]->key, key)) {
            hash_list_t *tmp = table->hash_table[index]->next;
            free(table->hash_table[index]->key);
            free(table->hash_table[index]);
            table->hash_table[index] = tmp;
            table->elements_count--;
            is_finded = true;
        } else {
            hash_list_t *prev = NULL;
            for (hash_list_t *current = table->hash_table[index]->next; current != NULL && !is_finded; current = current->next) {
                if (cmp_cnt) (*cmp_cnt)++;
                if (!strcmp(current->key, key)) {
                    if (!prev) table->hash_table[index] = current->next;
                    else prev->next = current->next;
                    table->elements_count--;
                    free(current->key);
                    free(current);
                    is_finded = true;
                } else
                    prev = current;
            }
        }
    }
    return !is_finded ? 0 : 1;
}

static void free_list(hash_list_t *list) {
    for (hash_list_t *current = list; !current;) {
        hash_list_t *tmp = current->next;
        free(current->key);
        free(current);
        current = tmp;
    }
}

void ohash_free(ohash_table_t *table) {
    for (size_t i = 0; i < table->size; i++)
        if (table->hash_table[i]) free_list(table->hash_table[i]);
    free(table->hash_table);
    table->size = 0;
}

void ohash_print(ohash_table_t *table) {
    if (!table->elements_count) return;
    printf("Opened hash table:\n");
    printf("----+\n");
    for (size_t i = 0; i < table->size; i++) {
        if (table->hash_table[i]) {
            printf( "%4zu| \033[0;33m%s\033[0m\n", i, table->hash_table[i]->key);
            for (hash_list_t *current = table->hash_table[i]->next; current; current = current->next) 
                printf( "%4s| \033[0;33m%s\033[0m\n", " ", current->key);
        } else {
            printf( "%4zu| empty\n", i);
        }
    }
    printf("----+\n");
}
