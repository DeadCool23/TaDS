#ifndef __DOT_H__
#define __DOT_H__

#include "tree.h"
#include <stdio.h>

extern char *dot_filename;

/**
 * @brief Функция записи дерева в DOT-файл
 * 
 * @param [in] dot_filename - имя DOT-файла
 * @param [in] root - Корневой элемент дерева
 * @return Код ошибки
 */
err_t generate_dot_file(const char *dot_filename, word_tree_t *root);
/**
 * @brief Пройедура открытия DOT-файла
 * @param [in] dot_filename - имя DOT-файла
 */
void print_tree_from_dot(const char *dot_filename);

#endif // __DOT_H__
