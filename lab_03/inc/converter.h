#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "errs.h"
#include "matrix.h"

err_t sparce_to_std_vec(const rvsparce_t *sparce_vec, rvector_t *std_vec);
err_t std_to_sparce_vec(const rvector_t *std_vec, rvsparce_t *sparce_vec);

err_t std_to_sparce_matrix(const matrix_t *std_mat, msparce_t *sparce_mat);

#endif //__CONVERTER_H__
