#include "allocate.h"

#include <stdlib.h>

void matrix_free(int **matrix, size_t n) {
    if (!matrix) return;
    for (size_t i = 0; i < n; free(matrix[i++]));
    free(matrix);
}

int **matrix_alloc(size_t n, size_t m) {
    int **matrix = malloc(n * sizeof(*matrix));
    if (!matrix) return NULL;
    for (size_t i = 0; i < n; i++) {
        matrix[i] = malloc(m * sizeof(**matrix));
        if (!matrix[i]) {
            matrix_free(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

int **matrix_calloc(size_t n, size_t m) {
    int **matrix = malloc(n * sizeof(*matrix));
    if (!matrix) return NULL;
    for (size_t i = 0; i < n; i++) {
        matrix[i] = calloc(m, sizeof(**matrix));
        if (!matrix[i]) {
            matrix_free(matrix, i);
            return NULL;
        }
    }
    return matrix;
}
