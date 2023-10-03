#include "sort.h"
#include "string.h"

void swap(void *fst, void *scd, size_t size) {
    char *cfst = fst, *cscd = scd;
    for (size_t i = 0; i < size; i++) {
        char tmp = *cfst;
        *(cfst++) = *cscd;
        *(cscd++) = tmp;
    }
}

void ssort_table(theatre_table_t *table) {
    bool swapped = true;
    for (size_t i = 0; i < table->size - 1 && swapped; i++) {
        swapped = false;
        for (size_t j = 0; j < table->size - i - 1; j++) {
            if (strcmp(table->theatres[j].theatre_name, table->theatres[j + 1].theatre_name) > 0) {
                swap(&table->theatres[j], &table->theatres[j + 1], sizeof(*table->theatres));
                swapped = true;
            }
        }
    }
}

void ssort_table_by_keys(key_table_t *key_table)  {
    bool swapped = true;
    size_t size = key_table->size;
    for (size_t i = 0; i < size - 1 && swapped; i++) {
        swapped = false;
        for (size_t j = 0; j < size - i - 1; j++) {
            if (strcmp(key_table->keys[j].theatre_name, key_table->keys[j + 1].theatre_name) > 0) {
                swap(&key_table->keys[j], &key_table->keys[j + 1], sizeof(*key_table->keys));
                swapped = true;
            }
        }
    }
}

int tpartition(theatre_t *arr, int low, int high) {
    theatre_t pivot = arr[high];

    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j].theatre_name, pivot.theatre_name) < 0) {
            i++;
            swap(&arr[i], &arr[j], sizeof(*arr));
        }
    }
    swap(&arr[i + 1], &arr[high], sizeof(*arr));
    return (i + 1);
}

void qsort_table(theatre_t *arr, int low, int high) {
    if (low < high) {
        int pi = tpartition(arr, low, high);
        
        qsort_table(arr, low, pi - 1);
        qsort_table(arr, pi + 1, high);
    }
}

int kpartition(key_t *arr, int low, int high) {
    key_t pivot = arr[high];

    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j].theatre_name, pivot.theatre_name) < 0) {
            i++;
            swap(&arr[i], &arr[j], sizeof(*arr));
        }
    }
    swap(&arr[i + 1], &arr[high], sizeof(*arr));
    return (i + 1);
}

void qsort_table_by_keys(key_t *arr, int low, int high) {
    if (low < high) {
        int pi = kpartition(arr, low, high);
        
        qsort_table_by_keys(arr, low, pi - 1);
        qsort_table_by_keys(arr, pi + 1, high);
    }
}
