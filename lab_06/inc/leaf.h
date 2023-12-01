#ifndef __LEAF_H__
#define __LEAF_H__

#include <stdio.h>

/// @brief Строка
typedef char *string;

/// @brief word_tree_t - дерево двоичного поиска слов
typedef struct leaf{
    string word;             // Слово
    struct leaf *left;       // Левый потомок
    struct leaf *right;      // Правый потомок
} word_tree_t;

/**
 * @brief Функция создания листа
 * 
 * @param [in] word - слово
 * @return Лист
 */
word_tree_t *leaf_create(const string word);
/**
 * @brief Процедура очистки листа
 * @param [in, out] leaf - Очищаемый лист
 */
void leaf_destroy(word_tree_t *leaf);

/**
 * @brief Процедура вывода листа
 * @param [in, out] file - Файловая переменная
 * @param [in] leaf - лист
 */
void leaf_print(FILE *file, word_tree_t *leaf);

/**
 * @brief Функция обмена словами
 * 
 * @param [in, out] fst - первое слово
 * @param [in, out] scd - второе слово
 */
void swap_words(char **fst, char **scd);

/* Компараторы */

int first_letter_cmp(const void *word, const void *letter);
int leafs_cmp(word_tree_t *fst, word_tree_t *scd);
int words_cmp(const void *fst, const void *scd);

#endif // __LEAF_H__
