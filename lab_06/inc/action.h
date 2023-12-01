#ifndef __ACTION_H__
#define __ACTION_H__

#include "errs.h"
#include "tree.h"

/**
 * @brief Интерфейс обхода дерева
 * 
 * @param [in] root - Корень дерева
 * @return Код ошибки 
 */
err_t traveral_act(word_tree_t *root);
/**
 * @brief Интерфейс загрузки данных в дерево
 * 
 * @param [out] root - Корень дерева
 * @param [out] filename - Имя файла
 * @return Код ошибки
 */
err_t load_act(word_tree_t **root, char **filename);
/**
 * @brief Интерфейс поиска слова в дереве
 * 
 * @param [in, out] root - Корень дерева
 * @return Код ошибки 
 */
err_t find_act(word_tree_t *root);
/**
 * @brief Интерфейс Удаления слов из дерева
 * 
 * @param [in, out] root - Корень дерева
 * @return Код ошибки 
 */
err_t del_act(word_tree_t **root);

#endif // __ACTION_H__
