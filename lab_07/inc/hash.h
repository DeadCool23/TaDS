#ifndef __HASH_H__
#define __HASH_H__

#include "leaf.h"
#include <stddef.h>
#include <stdbool.h>

#define BEG_SIZE 8 // Начальный размер хеш-таблицы

/// @brief chash_table_t - закрытая хеш-таблица
typedef struct {
    size_t size;                                          // Размер таблицы
    string *hash_table;                                   // Массив строк
    size_t elements_count;                                // Кол-во элементов в хеш-таблице
    size_t (*hash_func)(string key, size_t size);         // Хеш функция
} chash_table_t;


/// @brief hash_list_t - узел односвязного списка открытой хеш-таблицы
typedef struct hash_node {
    string key;                // Ключ
    size_t index;              // Индекс в односвязном списке
    struct hash_node *next;    // Указатель на следующий элемент
} hash_list_t;

/// @brief ohash_table_t - открытая хеш-таблица
typedef struct {
    size_t size;                                          // Размер таблицы
    size_t elements_count;                                // Кол-во элементов в хеш-таблице
    hash_list_t **hash_table;                             // Массив односвязных списков 
    size_t (*hash_func)(string key, size_t size);         // Хеш функция
} ohash_table_t;

void chash_init(chash_table_t *table, size_t size);
int chash_remove(chash_table_t *table, string key, size_t *cmp_cnt);
void chash_add(chash_table_t *table, string key, bool inf);
void crestructuring(chash_table_t *table, bool inf);
void chash_free(chash_table_t *table);
void chash_print(chash_table_t *table);

void ohash_init(ohash_table_t *table, size_t size);
int ohash_remove(ohash_table_t *table, string key, size_t *cmp_cnt);
void ohash_add(ohash_table_t *table, string key, bool inf);
void orestructuring(ohash_table_t *table, bool inf);
void ohash_free(ohash_table_t *table);
void ohash_print(ohash_table_t *table);

#endif // __HASH_H__
