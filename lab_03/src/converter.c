#include "converter.h"

#include <stdlib.h>
#include <stdbool.h>

err_t std_to_sparce_vec(const rvector_t *std_vec, rvsparce_t *sparce_vec) {
    err_t err = OK;
    void *ptmp = NULL;
    sparce_vec->els_cnt = 0;
    sparce_vec->els = malloc(0);
    sparce_vec->el_j = malloc(0);
    sparce_vec->size = std_vec->size;
    for (size_t i = 0; i < sparce_vec->size; i++) {
        if (std_vec->vector[i] != 0) {
            ptmp = realloc(sparce_vec->els, (sparce_vec->els_cnt + 1) * sizeof(*sparce_vec->els));
            if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
            sparce_vec->els = ptmp;
            //------
            ptmp = realloc(sparce_vec->el_j, (sparce_vec->els_cnt + 1) * sizeof(*sparce_vec->el_j));
            if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
            sparce_vec->el_j = ptmp;

            sparce_vec->els[sparce_vec->els_cnt] = std_vec->vector[i];
            sparce_vec->el_j[sparce_vec->els_cnt++] = i;
        }
    }
    return err;
    
    END:
    free(sparce_vec->els);
    free(sparce_vec->el_j);
    sparce_vec->els = NULL;
    sparce_vec->el_j = NULL;
    return err;
}

err_t sparce_to_std_vec(const rvsparce_t *sparce_vec, rvector_t *std_vec) {
    std_vec->size = sparce_vec->size;
    std_vec->vector = calloc(sparce_vec->size, sizeof(*std_vec->vector));
    if (!std_vec->vector) return ERR_MEM;
    for (size_t i = 0; i < sparce_vec->els_cnt; i++)
        std_vec->vector[sparce_vec->el_j[i]] = sparce_vec->els[i];
    return OK;
}

err_t std_to_sparce_matrix(const matrix_t *std_mat, msparce_t *sparce_mat) {
    err_t err = OK;
    void *ptmp = NULL;
    sparce_mat->els_cnt = 0;
    sparce_mat->rows = std_mat->rows;
    sparce_mat->cols = std_mat->cols;
    sparce_mat->el_j = malloc(sparce_mat->cols * sizeof(*sparce_mat->el_j));
    if (!sparce_mat->el_j) return ERR_MEM;
    for (size_t j = 0; j < sparce_mat->cols; j++) {
        bool is_zero = true;
        sparce_mat->el_j[j] = -1;
        for (size_t i = 0; i < sparce_mat->rows; i++) {
            if (std_mat->data[i][j]!= 0) {
                ptmp = realloc(sparce_mat->els, (sparce_mat->els_cnt + 1) * sizeof(*sparce_mat->els));
                if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
                sparce_mat->els = ptmp;
                //------
                ptmp = realloc(sparce_mat->el_i, (sparce_mat->els_cnt + 1) * sizeof(*sparce_mat->el_i));
                if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
                sparce_mat->el_i = ptmp;

                sparce_mat->els[sparce_mat->els_cnt] = std_mat->data[i][j];
                if (is_zero) sparce_mat->el_j[j] = sparce_mat->els_cnt;
                sparce_mat->el_i[sparce_mat->els_cnt] = i;
                sparce_mat->els_cnt++;
                is_zero = false;
            }
        }
    }
    return err;

    END:
    free(sparce_mat->el_j);
    sparce_mat->el_j = NULL;
    free(sparce_mat->el_i);
    sparce_mat->el_i = NULL;
    free(sparce_mat->el_j);
    sparce_mat->el_j = NULL;
    return err;
}
