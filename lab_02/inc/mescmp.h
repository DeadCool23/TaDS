#ifndef MESCMP_H
#define MESCMP_H

#include <stddef.h>

#include "theatre.h"

#define SORT_TYPE 2
#define ARR_TYPE 2

#define RUNS_CNT 1000

typedef enum {
    KEY_TABLE,
    TABLE
} arr_type_t;

typedef struct {
    size_t time;
    size_t mem;
} mes_t;


void get_measures(mes_t res[][ARR_TYPE], key_table_t *key_table, theatre_table_t *table);

void print_sort_cmp_table(mes_t res[][ARR_TYPE]);
void print_arr_cmp_table(mes_t res[][ARR_TYPE]);

#endif //MESCMP_H