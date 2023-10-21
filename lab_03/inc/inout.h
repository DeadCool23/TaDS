#ifndef __INOUT_H__
#define __INOUT_H__

#include <stdio.h>

#include "matrix.h"
#include "matrixio.h"
#include "vectorio.h"

typedef enum {
    STD_INPUT,
    COORDINATES,
} input_t;

#define INPUT_PROMT(file, ...)               \
    do {                                     \
        if (fileno(file) == STDIN_FILENO)    \
            fprintf(stdout, __VA_ARGS__);    \
    } while(0)

#define OUTPUT_INFO(file, ...)               \
    do {                                     \
        if (fileno(file) == STDOUT_FILENO)   \
            fprintf(stdout, __VA_ARGS__);    \
    } while(0)

err_t input_data(matrix_t *matrix, rvector_t *vector);
err_t output_data(const matrix_t *matrix, const msparce_t *smatrix,
                  const rvector_t *vector, const rvsparce_t *svector);

#endif //__INOUT_H__
