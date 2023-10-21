#include "helper.h"

#include "allocate.h"
#include "matrix.h"

#include <stdlib.h>
#include <string.h>

bool is_eof(FILE *file) {
    char end = getc(file);
    if (end == EOF)
        return true;
    if (end == '\n') {
        char endd = getc(file);
        if (endd == EOF)
            return true;
        ungetc(endd, file);
    }
    ungetc(end, file);
    return false;
}

void clean_buf(void) { for (char symb = getc(stdin); symb != '\n' && symb != EOF; symb = getc(stdin)); }

char *get_str(FILE *file, char end, ssize_t size) {
    size_t cnt = 0;
    char symb, *str = NULL;
    while ((symb = getc(file)) != end) {
        if (symb == EOF) goto NUL;
        char *ptmp = realloc(str, (cnt + 1) * sizeof(*str));
        if (!ptmp) return NULL;
        str = ptmp;
        *(str + cnt++) = symb;
        if (size >= 0 && size < (ssize_t)cnt) goto NUL;
    }

    char *ptmp = realloc(str, (cnt + 1) * sizeof(*str));
    if (!ptmp) return NULL;
    str = ptmp;
    *(str + cnt) = '\0';
    if (!strlen(str)) goto NUL;
    return str;

    NUL:
    free(str);
    return NULL;
}

void free_data(matrix_t *std_matrix, rvector_t *std_vec, 
               msparce_t *sparce_mat, rvsparce_t *sparce_vec) {
    matrix_free(std_matrix->data, std_matrix->rows);
    std_matrix->data = NULL;

    free(std_vec->vector);
    std_vec->vector = NULL;

    free(sparce_mat->el_i);
    sparce_mat->el_i = NULL;
    free(sparce_mat->el_j);
    sparce_mat->el_j = NULL;
    free(sparce_mat->els);
    sparce_mat->els = NULL;

    free(sparce_vec->el_j);
    sparce_vec->el_j = NULL;
    free(sparce_vec->els);
    sparce_vec->els = NULL;
}

void free_res(rvector_t *std_res, rvsparce_t *sparce_res) {
    free(std_res->vector);
    std_res->vector = NULL;

    free(sparce_res->el_j);
    sparce_res->el_j = NULL;
    free(sparce_res->els); 
    sparce_res->els = NULL; 
}