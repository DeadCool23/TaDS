#include "matrix_math.h"

#include <stdarg.h>
#include <stdlib.h>

#include "inout.h"
#include "mult_mes.h"

err_t std_mult(const rvector_t *fst, const matrix_t *scd, rvector_t *res, bool mes, ...) {
    if (fst->size != scd->rows) return ERR_MATH;

    va_list ap;
    va_start(ap, mes);
    size_t *time;
    if (mes) time = va_arg(ap, size_t (*));

    res->size = scd->cols;
    res->vector = malloc(res->size * sizeof(*res->vector));
    if (!res->vector) return ERR_MEM;

    size_t beg = microseconds_now();
    for (size_t i = 0; i < scd->cols; i++) {
        int sum_el = 0;
        for (size_t j = 0; j < fst->size; j++)
            sum_el += fst->vector[j] * scd->data[j][i];
        res->vector[i] = sum_el;
    }
    size_t end = microseconds_now();
    if (mes) *time = end - beg;

    va_end(ap);
    return OK;
}

err_t sparce_mult(const rvsparce_t *vector, const msparce_t *matrix, rvsparce_t *res, bool mes, ...) {
    if (vector->size != matrix->rows) return ERR_MATH;

    void *ptmp;
    err_t err = OK;

    res->els = NULL;
    res->el_j = NULL;
    res->els_cnt = 0;
    res->size = matrix->cols;

    va_list ap;
    va_start(ap, mes);
    size_t *time;
    if (mes) time = va_arg(ap, size_t (*));

    size_t trash_time = 0;
    size_t beg = microseconds_now();
    for (size_t i = 0; i < matrix->cols; i++) {
        if (matrix->el_j[i] == -1) continue;

        int el_sum = 0;
        
        size_t next_ind = i;
        AGAIN: next_ind++;
        if (next_ind != matrix->cols && matrix->el_j[next_ind] == -1) goto AGAIN;
        size_t next_col = next_ind != matrix->cols 
            ? (size_t)matrix->el_j[next_ind] 
            : matrix->els_cnt;
        
        for (size_t mj = matrix->el_j[i], vj = 0; mj < next_col && vj < vector->els_cnt; ) {
            if (vector->el_j[vj] == matrix->el_i[mj]) el_sum += vector->els[vj++] * matrix->els[mj++];
            else (vector->el_j[vj] < matrix->el_i[mj]) ? vj++ : mj++;
        }

        if (!el_sum) continue;
        size_t trash_beg = microseconds_now();
        ptmp = realloc(res->els, (res->els_cnt + 1) * sizeof(*res->els));
        if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
        res->els = ptmp;

        ptmp = realloc(res->el_j, (res->els_cnt + 1) * sizeof(*res->el_j));
        if (!ptmp) ERR_GOTO(err, ERR_MEM, END);
        res->el_j = ptmp;
        size_t trash_end = microseconds_now();
        trash_time += trash_end - trash_beg;

        res->els[res->els_cnt] = el_sum;
        res->el_j[res->els_cnt++] = i;
    }
    size_t end = microseconds_now();
    if (mes) *time = end - beg - trash_time;

    va_end(ap);
    return err;

    END:
    free(res->els);
    res->els = NULL;
    free(res->el_j);
    res->el_j = NULL;
    return err;
}
