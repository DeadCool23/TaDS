#ifndef __FILEPROCESS_H__
#define __FILEPROCESS_H__

#include "errs.h"
#include "tree.h"
#include "hash.h"

#include <stdbool.h>

typedef enum {
    GENERATE,
    LOAD,
} tree_load_t;

err_t word_read(FILE *file, string *word);

err_t prepare_file(char **filename, tree_load_t type);

err_t read_tree_from_file(word_tree_t **root, const char *filename);
err_t read_ohash_from_file(ohash_table_t *table, const char *filename, bool info);
err_t read_chash_from_file(chash_table_t *table, const char *filename, bool info);

err_t del_from_file(const char *filename, uni_cmp_t cmp, void *param, size_t *cmp_cnt);

#endif // __FILEPROCESS_H__
