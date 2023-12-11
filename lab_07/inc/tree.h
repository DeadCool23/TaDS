#ifndef __TREE_H__
#define __TREE_H__

#include "leaf.h"

typedef int (*uni_cmp_t)(const void *, const void *);

typedef enum {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER 
} traversal_t;

word_tree_t *tree_init(void);

word_tree_t *add_leaf(word_tree_t* root, word_tree_t *new_leaf);
word_tree_t *add_leaf_with_balance(word_tree_t *root, word_tree_t *new_leaf);
word_tree_t *copy_tree_with_balance(word_tree_t *dst_root, word_tree_t *src_root);
size_t leafs_cnt(word_tree_t *root);
word_tree_t *find_leaf(word_tree_t *root, uni_cmp_t cmp, const void *param, size_t *cmp_cnt);
word_tree_t *delete_leaf(word_tree_t *root, word_tree_t *del_leaf);
size_t delete_leafs(word_tree_t **root, uni_cmp_t cmp, const void *param);

void tree_destroy(word_tree_t* root);

#endif // __TREE_H__
