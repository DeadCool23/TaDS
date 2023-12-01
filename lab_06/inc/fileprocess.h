#ifndef __FILEPROCESS_H__
#define __FILEPROCESS_H__

#include "errs.h"
#include "tree.h"

typedef enum {
    GENERATE,
    LOAD,
} tree_load_t;

/**
 * @brief Считывание слова из файла
 * 
 * @param [in, out] file - файловая переменная
 * @param [out] word - слово
 * @return Код ошибки
 */
err_t word_read(FILE *file, string *word);
/**
 * @brief Функция подготовки данных для загрузки из файла
 * 
 * @param [out] filename - имя файла
 * @return Код ошибки
 */
err_t prepare_file(char **filename, tree_load_t type);
/**
 * @brief Функция считывания дерева из файла
 * 
 * @param [out] root - корень дерева 
 * @param [in] filename - имя файла
 * @return код ошибки
 */
err_t read_tree_from_file(word_tree_t **root, const char *filename);
/**
 * @brief Фунция удаления слов из файла
 * 
 * @param [in] filename - имя файла
 * @param [in] cmp - функция сравнения
 * @param [in] param - парметр сравнения
 * @return Код ошибки 
 */
err_t del_from_file(const char *filename, uni_cmp_t cmp, void *param);

#endif // __FILEPROCESS_H__
