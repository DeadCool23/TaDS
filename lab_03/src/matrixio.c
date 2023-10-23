#include "matrixio.h"

#include "allocate.h"
#include "inout.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static err_t coordinate_matrix_read(FILE *f, const matrix_t *matrix) {
#define CORDINATES_INPUT_CNT 3
    int els_cnt;
    err_t err = OK;
    bool *add_arr = calloc(matrix->rows * matrix->cols, sizeof(*add_arr));
    if (!add_arr) return ERR_MEM;

    INPUT_PROMT(f, "\nInput \033[4mcount of coordinates\033[0m: ");
    if (fscanf(f, "%d", &els_cnt) != 1)
        ERR_GOTO(err, ERR_IO, end);

    if (els_cnt < 0 || (size_t)els_cnt > matrix->rows * matrix->cols)
        ERR_GOTO(err, ERR_IO, end);

    for (size_t i = 0; i < (size_t)els_cnt; i++) {
        int el_tmp;
        int n_tmp, m_tmp;
        INPUT_PROMT(f, "\n");
        int *inputs[CORDINATES_INPUT_CNT] = { &n_tmp, &m_tmp, &el_tmp };
        char *promts[CORDINATES_INPUT_CNT] = { "Input row index: ", "Input column index: ", "Input value: " };
        for (size_t i = 0; i < CORDINATES_INPUT_CNT; i++) {
            INPUT_PROMT(f, "%s", promts[i]);
            if (fscanf(f, "%d", inputs[i]) != 1)
                ERR_GOTO(err, ERR_IO, end);
        }

        if ((n_tmp <= 0 || (size_t)n_tmp > matrix->rows) || 
            (m_tmp <= 0 || (size_t)m_tmp > matrix->cols) ||
            add_arr[matrix->cols * (n_tmp - 1) + (m_tmp - 1)])
            ERR_GOTO(err, ERR_IO, end);

        matrix->data[n_tmp - 1][m_tmp - 1] = el_tmp;
        add_arr[matrix->cols * (n_tmp - 1) + (m_tmp - 1)] = true;
    }
    
    end: free(add_arr);
    return !err ? (fileno(f) != STDIN_FILENO) ? (is_eof(f) ? OK : ERR_FILE) : OK : err;
#undef CORDINATES_INPUT_CNT
}

static err_t std_matrix_read(FILE *f, matrix_t *matrix) {
    err_t err = OK;
    INPUT_PROMT(f, "Input matrix:\n");
    for (size_t i = 0; i < matrix->rows && !err; i++)
        for (size_t j = 0; j < matrix->cols && !err; j++)
            if (scanf("%d", &matrix->data[i][j]) != 1)
                err = ERR_IO;

    return !err ? (fileno(f) != STDIN_FILENO) ? (is_eof(f) ? OK : ERR_FILE) : OK : err;
}

static err_t matrix_create(FILE *f, matrix_t *matrix, input_t type) {
    int n_tmp, m_tmp;
    INPUT_PROMT(f, "\n");
    INPUT_PROMT(f, "Input \033[4mrows\033[0m count: ");
    if (fscanf(f, "%d", &n_tmp) != 1) return ERR_IO;
    INPUT_PROMT(f, "Input \033[4mcolumns\033[0m count: ");
    if (fscanf(f, "%d", &m_tmp) != 1) return ERR_IO;
    
    if (n_tmp <= 0 || m_tmp <= 0) return ERR_IO;
    
    matrix->rows = n_tmp;
    matrix->cols = m_tmp;

    matrix->data = matrix_calloc(matrix->rows, matrix->cols);
    if (!matrix->data) return ERR_MEM;
    
    err_t err = OK;
    switch (type) {
        case COORDINATES:
            err = coordinate_matrix_read(f, matrix);
            break;
        case STD_INPUT:
            err = std_matrix_read(f, matrix);
            break;
        default:
            err = ERR_CMD;
            break;
    }

    if (err) matrix_free(matrix->data, matrix->rows);
    if (err) matrix->data = NULL;
    return err;
}

err_t matrix_input(matrix_t *matrix) {
    char tmp;
    input_t type;
    INPUT_PROMT(stdin, "Choose type of matrix input\n[S:Standart, C:Coordinates]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();
    switch (tmp) {
        case 'S':
            type = STD_INPUT;
            break;
        case 'C':
            type = COORDINATES;
            break;
        default:
            return ERR_CMD;
    }

    INPUT_PROMT(stdin, "\nEntire matrix from\n[S:Stdin, F:File]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();

    err_t err = OK;
    char *filename = NULL;
    switch (tmp) {
        case 'S':
            err = matrix_create(stdin, matrix, type);
            break;
        case 'F':
            INPUT_PROMT(stdin, "Input filename: ");
            if (!(filename = get_str(stdin, '\n', -1))) err = ERR_IO;
            if (!err) {
                FILE *file = fopen(filename, "r");
                if (file == NULL) err = ERR_FILE;
                if (!err) {
                    err = matrix_create(file, matrix, type);
                    fclose(file);
                }
            }
            free(filename);
            break;
        default:
            err = ERR_CMD;
            break;
    }
    return err;
}

static void std_matrix_print(FILE *file, const matrix_t *matrix) {
    OUTPUT_INFO(file, "\nMatrix sizes: "); 
    fprintf(file, "%zu %zu\n", matrix->rows, matrix->cols);
    
    OUTPUT_INFO(file, "Matrix:\n");
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0 ; j < matrix->cols; j++)
            fprintf(file, "%-4d ", matrix->data[i][j]);
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

static void sparce_matrix_print(FILE *file, const msparce_t *matrix) {
    OUTPUT_INFO(file, "\nMatrix sizes: "); 
    fprintf(file, "%zu %zu\n", matrix->rows, matrix->cols);

    OUTPUT_INFO(file, "\nEls: ");
    for (size_t i = 0; i < matrix->els_cnt; i++)
        fprintf(file, "%-4d ", matrix->els[i]);
    OUTPUT_INFO(file, "\nIEls: ");
    for (size_t i = 0; i < matrix->els_cnt; i++)
        fprintf(file, "%-4zu ", matrix->el_i[i]);
    OUTPUT_INFO(file, "\nJEls: ");
    for (size_t i = 0; i < matrix->cols; i++)
        fprintf(file, "%-4zd ", matrix->el_j[i]);
    fprintf(file, "\n");
}

err_t matrix_print(const matrix_t *matrix, const msparce_t *msparce) {
    char tmp;
    INPUT_PROMT(stdin, "Print \033[1mmatrix\033[0m\n[C:Classical, S:Sparce]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();
    switch (tmp) {
        case 'C':
            std_matrix_print(stdout, matrix);
            break;
        case 'S':
            sparce_matrix_print(stdout, msparce);
            break;
        default:
            return ERR_CMD;
    }
    return OK;
}
