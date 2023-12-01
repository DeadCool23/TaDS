#include "tree.h"
#include "errs.h"
#include "leaf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define final_leaf_destroy(leaf) do { leaf_destroy(leaf); free(leaf); } while(0)

word_tree_t *tree_init(void) { return NULL; }

word_tree_t *add_leaf(word_tree_t *root, word_tree_t *new_leaf) {
    if (root == NULL) return new_leaf;

    if (leafs_cmp(root, new_leaf) > 0)
        root->left = add_leaf(root->left, new_leaf);
    else if (leafs_cmp(root, new_leaf) < 0)
        root->right = add_leaf(root->right, new_leaf);

    return root;
}

size_t leafs_cnt(word_tree_t *root) { return (root) ? 1 + leafs_cnt(root->left) + leafs_cnt(root->right) : 0; }

word_tree_t *find_leaf(word_tree_t *root, uni_cmp_t cmp, const void *param) {
    if (!root) return NULL;

    int cmp_result = cmp(root->word, param);

    if (cmp_result > 0) return find_leaf(root->left, cmp, param);
    else if (cmp_result < 0) return find_leaf(root->right, cmp, param);
    else return root;
}

/**
 * @brief Функция поиска минимального листа в ДДП
 * 
 * @param [in, out] leaf - лист 
 * @return Минимальный лист
 */
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
    while ((del_leaf = find_leaf(*root, cmp, param))) {
        *root = delete_leaf(*root, del_leaf);
        dels_cnt++;
    }
    return dels_cnt;
}

/**
 * @brief Префиксный обход дерева
 * @param [in] root - Корень дерева 
 */
static void pre_order(word_tree_t *root) {
    if (!root) return;

    leaf_print(stdout, root);
    pre_order(root->left);
    pre_order(root->right);
}

/**
 * @brief Прямой обход дерева
 * @param [in] root - Корень дерева 
 */
static void in_order(word_tree_t *root) {
    if (!root) return;

    pre_order(root->left);
    leaf_print(stdout, root);
    pre_order(root->right);
}

/**
 * @brief Постфиксный обход дерева
 * @param [in] root - Корень дерева 
 */
static void post_order(word_tree_t *root) {
    if (!root) return;

    pre_order(root->left);
    pre_order(root->right);
    leaf_print(stdout, root);
}

err_t tree_traversal(word_tree_t *root, traversal_t order) {
    switch (order) {
        case PRE_ORDER:
            pre_order(root);
            break;
        case IN_ORDER:
            in_order(root);
            break;
        case POST_ORDER:
            post_order(root);
            break;
        default:
            return ERR_CMD;
    }
    return OK;
}

void tree_destroy(word_tree_t *root) {
    if (root == NULL) return;

    tree_destroy(root->left);
    tree_destroy(root->right);

    leaf_destroy(root);
    free(root);
}

#undef final_leaf_destroy
