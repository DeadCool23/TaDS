#ifndef __KEY_H__
#define __KEY_H__

#include "theatre.h"

void init(key_table_t *key);

void key_table_gen(key_table_t *key, theatre_table_t table);
void free_key_table(key_table_t *key_table);

void key_table_print(key_table_t *key_table, theatre_table_t *table);
void keys_print(key_table_t *key_table);

#endif //__KEY_H__

