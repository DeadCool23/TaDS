#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "errs.h"
#include "matrix.h"

#define FULL_PERCENT 100.0

void vec_gen(rvector_t *matrix, size_t percent);
void matrix_gen(matrix_t *matrix, size_t percent);

err_t generator(matrix_t *matrix, rvector_t *vector);

#endif // __GENERATOR_H__
