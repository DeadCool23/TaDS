#ifndef __LEAF_H__
#define __LEAF_H__

#include <stdio.h>

/// @brief Строка
typedef char *string;

/// @brief word_tree_t - дерево двоичного поиска слов
typedef struct leaf{
    string word;             // Слово
    size_t height;           // Высота в дереве
    struct leaf *left;       // Левый потомок
    struct leaf *right;      // Правый потомок
} word_tree_t;

word_tree_t *leaf_create(const string word);
word_tree_t *leaf_copy(word_tree_t *leaf);

void leaf_destroy(word_tree_t *leaf);

void leaf_print(FILE *file, word_tree_t *leaf);

void swap_words(char **fst, char **scd);

/* Компараторы */
int first_letter_cmp(const void *word, const void *letter);
int leafs_cmp(word_tree_t *fst, word_tree_t *scd);
int words_cmp(const void *fst, const void *scd);

#endif // __LEAF_H__
