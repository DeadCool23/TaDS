#ifndef __MATRIX_IO__
#define __MATRIX_IO__

#include <stdio.h>

#include "errs.h"
#include "matrix.h"

err_t matrix_input(matrix_t *matrix);
err_t matrix_print(const matrix_t *matrix, const msparce_t *smatrix);

#endif //__MATRIX_IO__

