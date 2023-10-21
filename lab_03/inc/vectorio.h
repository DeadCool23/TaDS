#ifndef __VECTORIO_H__
#define __VECTORIO_H__

#include <stdio.h>

#include "errs.h"
#include "matrix.h"

err_t vector_input(rvector_t *vector);
err_t vector_print(const rvector_t *vector, const rvsparce_t *msparce);

#endif //__VECTORIO_H__
