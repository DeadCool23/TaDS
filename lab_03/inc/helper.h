#ifndef __HELPER_H__
#define __HELPER_H__

#include "matrix.h"

#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>

bool is_eof(FILE *file);

void clean_buf(void);

char *get_str(FILE *file, char end, ssize_t size);

void free_res(rvector_t *std_res, rvsparce_t *sparce_res);
void free_data(matrix_t *std_matrix, rvector_t *std_vec,
               msparce_t *sparce_mat, rvsparce_t *sparce_vec);

#endif //__HELPER_H__