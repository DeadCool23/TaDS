#include "generator.h"

#include "inout.h"
#include "allocate.h"

#include <time.h>
#include <math.h>

#include <stdlib.h>

void matrix_gen(matrix_t *matrix, size_t percent) {
    srand(time(NULL));

    size_t total_elements = matrix->rows * matrix->cols;
    size_t non_zero_elements = (size_t)round((percent * total_elements) / FULL_PERCENT);

    size_t row, col;
    for (size_t i = 0; i < non_zero_elements; i++) {
        ADD_EL:
        row = rand() % matrix->rows;
        col = rand() % matrix->cols;
        if (matrix->data[row][col]) goto ADD_EL;
        matrix->data[row][col] = (total_elements - 1) 
        ? rand() % (total_elements - 1) + 1
        : 1;
    }
}

void vec_gen(rvector_t *vector, size_t percent) {
    srand(time(NULL));

    size_t total_elements = vector->size;
    size_t non_zero_elements = (size_t)round((percent * total_elements) / FULL_PERCENT);

    size_t ind;
    for (size_t i = 0; i < non_zero_elements; i++) {
        ADD_EL:
        ind = rand() % vector->size;
        if (vector->vector[ind]) goto ADD_EL;
        vector->vector[ind] = (total_elements - 1) 
        ? rand() % (total_elements - 1) + 1
        : 1;
    }
}

err_t generator(matrix_t *matrix, rvector_t *vector) {
    int n_tmp, m_tmp;
    INPUT_PROMT(stdin, "\n");
    INPUT_PROMT(stdin, "Input \033[4mrows\033[0m count: ");
    if (fscanf(stdin, "%d", &n_tmp) != 1) return ERR_IO;
    INPUT_PROMT(stdin, "Input \033[4mcolumns\033[0m count: ");
    if (fscanf(stdin, "%d", &m_tmp) != 1) return ERR_IO;
    
    if (n_tmp <= 0 || m_tmp <= 0) return ERR_IO;
    
    matrix->rows = n_tmp;
    matrix->cols = m_tmp;

    err_t err = OK;
    matrix->data = matrix_calloc(matrix->rows, matrix->cols);
    if (!matrix->data) return ERR_MEM;
    vector->size = matrix->rows;
    vector->vector = calloc(vector->size, sizeof(*vector->vector));
    if (!vector->vector) ERR_GOTO(err, ERR_MEM, END);

    INPUT_PROMT(stdin, "Enter percent of filling the matrix with non-zero numbers\n[Format: num%%, where 0 <= num <= 100]: ");
    int percent; char tmp;
    if (fscanf(stdin, "%d%c", &percent, &tmp)!= 2) ERR_GOTO(err, ERR_IO, END);
    if (percent < 0 || percent > 100 || tmp != '%') ERR_GOTO(err, ERR_IO, END);

    vec_gen(vector, percent);
    matrix_gen(matrix, percent);

    INPUT_PROMT(stdin, "\n");

    return err;

    END:
    if (tmp == '\n') ungetc(tmp, stdin);
    matrix_free(matrix->data, matrix->rows);
    matrix->data = NULL;
    free(vector->vector);
    vector->vector = NULL;
    return err;
}

