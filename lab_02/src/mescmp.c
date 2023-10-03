#include "mescmp.h"
#include "time_calc.h"
#include "sort.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static size_t get_theatre_qsort_mes(theatre_table_t *table) {
    if (!table->size) return 0;
    theatre_table_t tmp = {
        .size = table->size, 
        .theatres = malloc(table->size * sizeof(*table->theatres))
        };
    
    size_t runs = 0, sum = 0;
    while (runs++ < RUNS_CNT) {
        memcpy(tmp.theatres, table->theatres, tmp.size * sizeof(*tmp.theatres));
        size_t beg = microseconds_now();
        qsort_table(tmp.theatres, 0, tmp.size - 1);
        size_t end = microseconds_now();
        sum += (end - beg) * 1000;
    }
    free(tmp.theatres);
    return (sum / runs);
}

static size_t get_theatre_ssort_mes(theatre_table_t *table) {
    if (!table->size) return 0;
    theatre_table_t tmp = {
        .size = table->size, 
        .theatres = malloc(table->size * sizeof(*table->theatres))
        };
    
    size_t runs = 0, sum = 0;
    while (runs++ < RUNS_CNT) {
        memcpy(tmp.theatres, table->theatres, tmp.size * sizeof(*tmp.theatres));
        size_t beg = microseconds_now();
        ssort_table(&tmp);
        size_t end = microseconds_now();
        sum += (end - beg) * 1000;
    }
    free(tmp.theatres);
    return (sum / runs);
}

static size_t get_key_qsort_mes(key_table_t *key_table) {
    if (!key_table->size) return 0;
    key_table_t tmp = {
        .size = key_table->size, 
        .keys = malloc(key_table->size * sizeof(*key_table->keys))
        };
    
    size_t runs = 0, sum = 0;
    while (runs++ < RUNS_CNT) {
        memcpy(tmp.keys, key_table->keys, tmp.size * sizeof(*tmp.keys));
        size_t beg = microseconds_now();
        qsort_table_by_keys(tmp.keys, 0, tmp.size - 1);
        size_t end = microseconds_now();
        sum += (end - beg) * 1000;
    }
    free(tmp.keys);
    return (sum / runs);
}

static size_t get_key_ssort_mes(key_table_t *key_table) {
    if (!key_table->size) return 0;
    key_table_t tmp = {
        .size = key_table->size, 
        .keys = malloc(key_table->size * sizeof(*key_table->keys))
        };
    
    size_t runs = 0, sum = 0;
    while (runs++ < RUNS_CNT) {
        memcpy(tmp.keys, key_table->keys, tmp.size * sizeof(*tmp.keys));
        size_t beg = microseconds_now();
        ssort_table_by_keys(&tmp);
        size_t end = microseconds_now();
        sum += (end - beg) * 1000;
    }
    free(tmp.keys);
    return (sum / runs);
}

void get_measures(mes_t res[][ARR_TYPE], key_table_t *key_table, theatre_table_t *table) {
    double  j = 0;
    for (size_t i = 0; i < ARR_TYPE + SORT_TYPE; i++, j += 0.5) {
        if (i < 2) {
            if (i % 2 == KEY_TABLE) {
                res[(size_t)j][KEY_TABLE].time = get_key_ssort_mes(key_table);
                res[(size_t)j][KEY_TABLE].mem = sizeof(*key_table->keys) * key_table->size;
            } else {
                res[(size_t)j][TABLE].time = get_theatre_ssort_mes(table);
                res[(size_t)j][TABLE].mem = sizeof(*table->theatres) * table->size;
            }
        } else {
            if (i % 2 == KEY_TABLE) {
                res[(size_t)j][KEY_TABLE].time = get_key_qsort_mes(key_table);
                res[(size_t)j][KEY_TABLE].mem = sizeof(*key_table->keys) * key_table->size;
            } else {
                res[(size_t)j][TABLE].time = get_theatre_qsort_mes(table);
                res[(size_t)j][TABLE].mem = sizeof(*table->theatres) * table->size;
            }
        }
    }
}

void print_sort_cmp_table(mes_t res[][ARR_TYPE]) {
    printf("%-21s|%-30s|%-30s|\n", " ", "ssort", "qsort" );
    printf("---------------------|------------------------------|------------------------------|\n");
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", "Keys table", "Time, ms", res[0][KEY_TABLE].time, res[1][KEY_TABLE].time);
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", " ", "Memory, b", res[0][KEY_TABLE].mem, res[1][KEY_TABLE].mem);
    printf("----------|----------|------------------------------|------------------------------|\n");
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", "Table", "Time, ms", res[0][TABLE].time, res[1][TABLE].time);
    printf("%-10s|%-10s|%-30zu|%-30zu|\n\n", " ", "Memory, b", res[0][TABLE].mem, res[1][TABLE].mem);
}

void print_arr_cmp_table(mes_t res[][ARR_TYPE]) {
    printf("%-21s|%-30s|%-30s|\n", " ", "Table", "Keys table" );
    printf("---------------------|------------------------------|------------------------------|\n");
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", "qsort", "Time, ms", res[1][TABLE].time, res[1][KEY_TABLE].time);
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", " ", "Memory, b", res[1][TABLE].mem, res[1][KEY_TABLE].mem);
    printf("----------|----------|------------------------------|------------------------------|\n");
    printf("%-10s|%-10s|%-30zu|%-30zu|\n", "ssort", "Time, ms", res[0][TABLE].time, res[0][KEY_TABLE].time);
    printf("%-10s|%-10s|%-30zu|%-30zu|\n\n", " ", "Memory, b", res[0][TABLE].mem, res[0][KEY_TABLE].mem);
}
