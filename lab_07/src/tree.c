#include "tree.h"
#include "leaf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define final_leaf_destroy(leaf) do { leaf_destroy(leaf); free(leaf); } while(0)

word_tree_t *tree_init(void) { return NULL; }

static size_t height(word_tree_t *leaf) {
    if (leaf == NULL) return 0;
    return leaf->height;
}

static size_t max_height(size_t h1, size_t h2) { return (h1 > h2) ? h1 : h2; }

static word_tree_t *right_rotate(word_tree_t *y) {
    word_tree_t *x = y->left;
    word_tree_t *tmp = x->right;

    x->right = y;
    y->left = tmp;

    y->height = max_height(height(y->left), height(y->right)) + 1;
    x->height = max_height(height(x->left), height(x->right)) + 1;

    return x;
}

static word_tree_t *left_rotate(word_tree_t *x) {
    word_tree_t *y = x->right;
    word_tree_t *tmp = y->left;

    y->left = x;
    x->right = tmp;

    x->height = max_height(height(x->left), height(x->right)) + 1;
    y->height = max_height(height(y->left), height(y->right)) + 1;

    return y;
}

static int get_balance(word_tree_t *leaf) {
    if (!leaf) return 0;
    return (int)height(leaf->left) - (int)height(leaf->right);
}

static word_tree_t *balance_tree(word_tree_t *root) {
    if (root == NULL)
        return NULL;

    root->height = 1 + max_height(height(root->left), height(root->right));

    int balance = get_balance(root);

    if (balance > 1) {
        if (get_balance(root->left) >= 0) {
            return right_rotate(root);
        } else {
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }
    }

    if (balance < -1) {
        if (get_balance(root->right) <= 0) {
            return left_rotate(root);
        } else {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }

    return root;
}

word_tree_t *add_leaf(word_tree_t *root, word_tree_t *new_leaf) {
    if (root == NULL) {
        new_leaf->height = 1;
        return new_leaf;
    }

    if (leafs_cmp(root, new_leaf) > 0)
        root->left = add_leaf(root->left, new_leaf);
    else if (leafs_cmp(root, new_leaf) < 0)
        root->right = add_leaf(root->right, new_leaf);

    root->height = 1 + max_height(height(root->left), height(root->right));
    return root;
}

word_tree_t *add_leaf_with_balance(word_tree_t *root, word_tree_t *new_leaf) {
    if (!root) {
        new_leaf->height = 1;
        return new_leaf;
    }

    if (leafs_cmp(root, new_leaf) > 0)
        root->left = add_leaf_with_balance(root->left, new_leaf);
    else if (leafs_cmp(root, new_leaf) < 0)
        root->right = add_leaf_with_balance(root->right, new_leaf);

    root->height = 1 + max_height(height(root->left), height(root->right));
    return balance_tree(root);
}

word_tree_t *copy_tree_with_balance(word_tree_t *dst_root, word_tree_t *src_root) {
    if (!src_root) return dst_root;

    dst_root = copy_tree_with_balance(dst_root, src_root->left);
    dst_root = copy_tree_with_balance(dst_root, src_root->right);
    dst_root = add_leaf_with_balance(dst_root, leaf_copy(src_root));

    return dst_root;
}

size_t leafs_cnt(word_tree_t *root) { return (root) ? 1 + leafs_cnt(root->left) + leafs_cnt(root->right) : 0; }

word_tree_t *find_leaf(word_tree_t *root, uni_cmp_t cmp, const void *param, size_t *cmp_cnt) {
    if (!root) return NULL;

    int cmp_result = cmp(root->word, param);
    if (cmp_cnt) (*cmp_cnt)++;

    if (cmp_result > 0) return find_leaf(root->left, cmp, param, cmp_cnt);
    else if (cmp_result < 0) return find_leaf(root->right, cmp, param, cmp_cnt);
    else return root;
}

static word_tree_t *find_min(word_tree_t *leaf) {
    for (; leaf->left; leaf = leaf->left);
    return leaf;
}

word_tree_t *delete_leaf(word_tree_t *root, word_tree_t *del_leaf) {
    if (root == NULL || del_leaf == NULL)
        return root;

    if (leafs_cmp(root, del_leaf) > 0) {
        root->left = delete_leaf(root->left, del_leaf);
    } else if (leafs_cmp(root, del_leaf) < 0) {
        root->right = delete_leaf(root->right, del_leaf);
    } else {
        if (root->left == NULL) {
            word_tree_t *temp = root->right;
            leaf_destroy(root);
            return temp;
        } else if (root->right == NULL) {
            word_tree_t *temp = root->left;
            leaf_destroy(root);
            return temp;
        } else {
            word_tree_t *temp = find_min(root->right);
            swap_words(&root->word, &temp->word);
            root->right = delete_leaf(root->right, temp);
        }
    }

    return root;
}

size_t delete_leafs(word_tree_t **root, uni_cmp_t cmp, const void *param) {
    size_t dels_cnt = 0;
    word_tree_t *del_leaf = NULL;
    while ((del_leaf = find_leaf(*root, cmp, param, NULL))) {
        *root = delete_leaf(*root, del_leaf);
        dels_cnt++;
    }
    return dels_cnt;
}

void tree_destroy(word_tree_t *root) {
    if (root == NULL) return;

    tree_destroy(root->left);
    tree_destroy(root->right);

    leaf_destroy(root);
    free(root);
}

#undef final_leaf_destroy
