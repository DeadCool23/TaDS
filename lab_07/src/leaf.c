#include "leaf.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

word_tree_t *leaf_create(const string word) {
    string tmp_word = strdup(word);
    if (!tmp_word) return NULL;
    word_tree_t *leaf = malloc(sizeof(*leaf));
    if (!leaf) {
        free(tmp_word);
        return NULL;
    }

    leaf->word = tmp_word;
    leaf->right = NULL;
    leaf->left = NULL;
    leaf->height = 1;
    return leaf;
}

word_tree_t *leaf_copy(word_tree_t *leaf) {
    if (!leaf) return NULL;
    word_tree_t *new_leaf = leaf_create(leaf->word);
    if (!new_leaf) return NULL;
    new_leaf->height = leaf->height;
    return new_leaf;
}

void swap_words(char **fst, char **scd) {
    char *tmp = *fst;
    *fst = *scd;
    *scd = tmp;
}

void leaf_print(FILE *file, word_tree_t *leaf) { fprintf(file, "%s->%zu\n", leaf->word, leaf->height); }

int words_cmp(const void *fst, const void *scd) { return strcmp(fst, scd); }
int first_letter_cmp(const void *word, const void *letter) { return ((char *)word)[0] - *(char *)letter; }
int leafs_cmp(word_tree_t *fst, word_tree_t *scd) { return strcmp(fst->word, scd->word); }

void leaf_destroy(word_tree_t *leaf) { free(leaf->word); }
