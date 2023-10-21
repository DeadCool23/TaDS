#include "mult_mes.h"

#include <unistd.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "allocate.h"
#include "converter.h"
#include "generator.h"
#include "matrix_math.h"

//-----------------------------------------------------------------------------------

#define UNDER_SIZE "----------"
#define UNDER_SPARCENESS "-------------"
#define UNDER_MEASURES "---------"
#define UNDER_STD_MATRIX "-------------"
#define UNDER_SPARCE_MATRIX "-------------"

#define SIZE_FORMAT_LEN 10
#define SPARCE_FORMAT_LEN 13

#define TABLE_SIZE 64

#define BETWEEN_SIZES printf("|%-10s|%-13s|%-9s|%-13s|%-13s|\n", \
                             UNDER_SIZE, UNDER_SPARCENESS, UNDER_MEASURES, \
                             UNDER_STD_MATRIX, UNDER_SPARCE_MATRIX)

#define BETWEEN_SPARCENESS printf("|%-10s|%-13s|%-9s|%-13s|%-13s|\n", \
                                  " ", UNDER_SPARCENESS, UNDER_MEASURES, \
                                  UNDER_STD_MATRIX, UNDER_SPARCE_MATRIX)

//-----------------------------------------------------------------------------------

#define MES_CNT 1000
#define MES_TYPES_CNT 2

typedef enum {
    STD_MES,
    SPARCE_MES,
} mes_types_t;

//-----------------------------------------------------------------------------------

ssize_t microseconds_now(void) {
    struct timeval val;

    if (gettimeofday(&val, 0))
        return (ssize_t) -1;

    return val.tv_sec * 1000ULL * 1000ULL + val.tv_usec;
}


// Size     |Sparceness, %|Measures|   Std matrix|Sparce matrix|
// ---------|-------------|--------|-------------|-------------|
// 1000x1000| 100%        |Mem, b  |             |             |
//          |             |Time, ms|             |             |
//          |-------------|--------|-------------|-------------|
// ...

// 10x10, 50x50, 100x100, 500x500, 1000x1000
// 0, 25, 50, 75, 100

static void liner(void) { 
    for (size_t i = 0; i < TABLE_SIZE; i++) printf("-");
    printf("\n");
}

static void header(void) {
    liner();
    printf("|\033[0;34m%-10s\033[0m|\033[0;34m%-13s\033[0m|\033[0;34m%-9s\033[0m|\033[0;34m%-13s\033[0m|\033[0;34m%-13s\033[0m|\n",
    "Size", "Sparceness, %", "Measures", "Std matrix", "Sparce matrix");
    printf("|%-10s|%-13s|%-9s|%-13s|%-13s|\n", UNDER_SIZE, UNDER_SPARCENESS, UNDER_MEASURES, UNDER_STD_MATRIX, UNDER_SPARCE_MATRIX);
}

// std_getter--------------------------------------------------------------------------------------------------------------

static size_t get_std_size(const matrix_t *matrix, const rvector_t *vector, const rvector_t *res_vec) {
    return sizeof(matrix->cols) + sizeof(matrix->rows) + 
    matrix->rows * sizeof(*matrix->data) + (matrix->cols * matrix->rows) * sizeof(**matrix->data) + 
    sizeof(vector->size) + vector->size * sizeof(*vector->vector) +
    sizeof(res_vec->size) + res_vec->size * sizeof(*res_vec->vector);
}

static err_t get_std_mes(const matrix_t *matrix, const rvector_t *vector, rvector_t *res_vec, mes_t *mes) {
    err_t err = OK;
    size_t times_sum = 0;
    for (size_t i = 1; i <= MES_CNT; i++) {
        size_t time = 0;
        if ((err = std_mult(vector, matrix, res_vec, true, &time))) goto END;
        times_sum += time;
        if (i == MES_CNT) mes->mem = get_std_size(matrix, vector, res_vec);
        goto END; BACK:;
    }
    mes->time = times_sum / MES_CNT /* mc */;
    return err;

    END:
    free(res_vec->vector);
    res_vec->vector = NULL;
    if (!err) goto BACK;
    return err;
}

// sparce_getter---------------------------------------------------------------------------------------------------------------

static size_t get_sparce_size(const msparce_t *matrix, const rvsparce_t *vector, const rvsparce_t *res_vec) {
    return sizeof(matrix->cols) + sizeof(matrix->rows) + sizeof(matrix->els_cnt) +
    matrix->rows * sizeof(*matrix->el_j) + matrix->els_cnt * (sizeof(*matrix->els) + sizeof(*matrix->el_i)) +
    sizeof(vector->size) + sizeof(vector->els_cnt) + vector->els_cnt * (sizeof(*vector->els) + sizeof(*vector->el_j)) +
    sizeof(res_vec->size) + sizeof(res_vec->els_cnt) + res_vec->els_cnt * (sizeof(*res_vec->els) + sizeof(*res_vec->el_j));
}

static err_t get_sparce_mes(const msparce_t *matrix, const rvsparce_t *vector, rvsparce_t *res_vec, mes_t *mes) {
    err_t err = OK;
    size_t times_sum = 0;
    for (size_t i = 1; i <= MES_CNT; i++) {
        size_t time = 0;
        if ((err = sparce_mult(vector, matrix, res_vec, true, &time))) goto END;
        times_sum += time;
        if (i == MES_CNT) mes->mem = get_sparce_size(matrix, vector, res_vec);
        goto END; BACK:;
    }
    mes->time = (times_sum / MES_CNT) /* mc */;
    return err;

    END:
    free(res_vec->els);
    res_vec->els = NULL;
    free(res_vec->el_j);
    res_vec->el_j = NULL;
    if (!err) goto BACK;
    return err;
}

//-----------------------------------------------------------------------------------

static err_t get_mes(size_t size, size_t percent, mes_t mess[MES_TYPES_CNT]) {
    err_t err = OK;
    size_t fill = (size_t)FULL_PERCENT - percent;

    matrix_t std_matrix = {
        .cols = size,
        .rows = size,
        .data = matrix_calloc(size, size)
    };
    if (!std_matrix.data) ERR_GOTO(err, ERR_MEM, std_mat);

    rvector_t std_vec = {
        .size = size,
        .vector = calloc(size, sizeof(*std_vec.vector))
    };
    if (!std_vec.vector) ERR_GOTO(err, ERR_MEM, std_vec);

    msparce_t sparce_mat = {
        .els = NULL,
        .el_i = NULL,
        .el_j = NULL,
    };
    rvsparce_t sparce_vec = {
        .els = NULL,
        .el_j = NULL,
    };

    rvector_t std_res = { .vector = NULL };
    rvsparce_t sparce_res = { .el_j = NULL, .els = NULL };

    matrix_gen(&std_matrix, fill);
    vec_gen(&std_vec, fill);

    err = std_to_sparce_matrix(&std_matrix, &sparce_mat);
    if (err) goto sparce_mat;
    err = std_to_sparce_vec(&std_vec, &sparce_vec);
    if (err) goto sparce_vec;

    err = get_std_mes(&std_matrix, &std_vec, &std_res, mess + STD_MES);
    if (err) goto END;
    err = get_sparce_mes(&sparce_mat, &sparce_vec, &sparce_res, mess + SPARCE_MES);
    if (err) goto END;

    END:
    sparce_vec: free(sparce_vec.els); free(sparce_vec.el_j);
    sparce_mat: free(sparce_mat.el_j); free(sparce_mat.el_i); free(sparce_mat.els);
    std_vec: free(std_vec.vector);
    std_mat: matrix_free(std_matrix.data, std_matrix.rows);
    return err;
}

err_t mes_table(void) {
    header();
    int step = 5;
    err_t err = OK;
    mes_t mess[MES_TYPES_CNT];
    for (size_t size = 10; size <= 1000; size *= step, step = step & 1 ? 2 : 5) {
        char size_format[SIZE_FORMAT_LEN + 1];
        snprintf(size_format, SIZE_FORMAT_LEN, "%zux%zu", size, size);
        for (size_t percent = 0; percent <= 100; percent += 25) {
            char percent_format[SPARCE_FORMAT_LEN + 1];
            snprintf(percent_format, SPARCE_FORMAT_LEN, "%zu%%", percent);
            err = get_mes(size, percent, mess);
            if (err) goto END;
            if (!percent) printf("|%-10s|%-13s|%-9s|%-13zu|%-13zu|\n", size_format, percent_format, "Mem, b", mess[STD_MES].mem, mess[SPARCE_MES].mem);
            else printf("|%-10s|%-13s|%-9s|%-13zu|%-13zu|\n", " ", percent_format, "Mem, b", mess[STD_MES].mem, mess[SPARCE_MES].mem);
            printf("|%-10s|%-13s|%-9s|%-13zu|%-13zu|\n", " ", " ", "Time, mcs", mess[STD_MES].time, mess[SPARCE_MES].time);
            if (percent != 100) BETWEEN_SPARCENESS;
        }
        if (size != 1000) BETWEEN_SIZES;
    }
    liner(); printf("\n\n");
    END: return err;
}