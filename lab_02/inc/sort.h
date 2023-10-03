#ifndef __SORT_H__
#define __SORT_H__

#include "theatre.h"

void swap(void *fst, void *scd, size_t size);

void ssort_table(theatre_table_t *table);
void ssort_table_by_keys(key_table_t *table);

void qsort_table(theatre_t *arr, int low, int high);
void qsort_table_by_keys(key_t *arr, int low, int high);

#endif //__SORT_H__