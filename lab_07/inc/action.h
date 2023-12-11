#ifndef __ACTION_H__
#define __ACTION_H__

#include "errs.h"
#include "tree.h"
#include "hash.h"

err_t add_act(word_tree_t **root, word_tree_t **avl_root, ohash_table_t *otable, chash_table_t *ctable);
err_t load_act(word_tree_t **root, word_tree_t **avl_root, ohash_table_t *otable, chash_table_t *ctable, char **filename);

err_t find_act(word_tree_t *root);

err_t del_tree_act(word_tree_t **root, word_tree_t **avl_root);
err_t del_hash_act(chash_table_t *ctable, ohash_table_t *otable);

#endif // __ACTION_H__
