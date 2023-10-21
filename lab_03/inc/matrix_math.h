#ifndef __MATRIX_MATH_H__
#define __MATRIX_MATH_H__

#include "errs.h"
#include "matrix.h"

#include <stdbool.h>

err_t std_mult(const rvector_t *fst, const matrix_t *scd, rvector_t *res, bool mes, ...);
err_t sparce_mult(const rvsparce_t *vector, const msparce_t *matrix, rvsparce_t *res, bool mes, ...);

#endif
