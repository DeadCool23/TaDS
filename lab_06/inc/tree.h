#ifndef __TREE_H__
#define __TREE_H__

#include "errs.h"
#include "leaf.h"

typedef int (*uni_cmp_t)(const void *, const void *);

typedef enum {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER 
} traversal_t;

/**
 * @brief Функция инициализации дерева
 * @return Корень дерева
 */
word_tree_t *tree_init(void);
/**
 * @brief Функция добавление листа в дерево
 * 
 * @param [in, out] root - корень дерева 
 * @param [in] new_leaf - добавляемый лист
 * @return Коорень дерева 
 */
word_tree_t *add_leaf(word_tree_t* root, word_tree_t *new_leaf);
/**
 * @brief Функция подсчита листов в дереве
 * 
 * @param [in] root - корень дерева  
 * @return Количество листов в дереве
 */
size_t leafs_cnt(word_tree_t *root);
/**
 * @brief Функция поиска листа
 * @details Универсальный поиск
 * 
 * @param [in, out] root - Корень дерева
 * @param [in] cmp - компаратор для поиска
 * @param [in] param - искомый параметр 
 * @return Корень дерева
 */
word_tree_t* find_leaf(word_tree_t* root, uni_cmp_t cmp, const void *param);
/**
 * @brief Функция удаления всех листов по определенному признаку
 * 
 * @param [in, out] root - Корень дерева
 * @param [in] cmp - компаратор для поиска
 * @param [in] param - признак
 * @return Количество удаленных узлов
 */
size_t delete_leafs(word_tree_t **root, uni_cmp_t cmp, const void *param);
/**
 * @brief Процедура вывода обходов дерева
 * 
 * @param [in] root - Корень дерева
 * @param [in] order - порядок обхода 
 * @return Код ошибки 
 */
err_t tree_traversal(word_tree_t *root, traversal_t order);
/**
 * @brief Процедура очистки дерева
 * @param root - корень дерева
 */
void tree_destroy(word_tree_t* root);

#endif // __TREE_H__
