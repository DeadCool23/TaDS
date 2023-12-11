#ifndef __DOT_H__
#define __DOT_H__

#include "tree.h"
#include "errs.h"
#include <stdio.h>

extern const char *dot_filename;
extern const char *balanced_dot_filename;

err_t generate_dot_file(const char *dot_filename, word_tree_t *root, const char *tree_name);
void print_tree_from_dot(const char *dot_filename);

#endif // __DOT_H__
