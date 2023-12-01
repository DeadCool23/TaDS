#include "action.h"

#include "menu.h"
#include "helper.h"
#include "fileprocess.h"

#include <ctype.h>
#include <stdlib.h>

err_t traveral_act(word_tree_t *root) {
    if (!root) return ERR_EMPTY_TREE;

    int type;
    traversal_menu();
    SCANF_CHECKER(1, "%d", &type);
    printf("\n\n--\033[0;34mTraversal\033[0m--\n");
    err_t err = tree_traversal(root, type - 1);
    if (!err) printf("--\033[0;34mEnd traversal\033[0m--\n\n");
    return err;
}

static void free_filename(char **filename) {
    free(*filename);
    *filename = NULL;
}

err_t load_act(word_tree_t **root, char **filename) {
    if (*root) tree_destroy(*root);
    *root = NULL;

    free_filename(filename);

    int type;
    data_menu();
    SCANF_CHECKER(1, "%d", &type);
    clean_buf();
    err_t err = prepare_file(filename, type - 1);
    err = !err ? read_tree_from_file(root, *filename) : err;
    if (err) free_filename(filename);
    else printf("\n\nData \033[0;32msuccessfully\033[0m loaded to tree\n\n");
    return err;
}

err_t find_act(word_tree_t *root) {
    if (!root) return ERR_EMPTY_TREE;

    err_t err = OK;
    string searching_word;
    printf("Input searching word: ");
    err = word_read(stdin, &searching_word);    
    if (err) return err;

    word_tree_t *leaf = find_leaf(root, words_cmp, searching_word);
    if (!leaf) printf("Word \033[3m\"%s\"\033[0m \033[0;31mnot founded\033[0m\n\n", searching_word);
    else printf("Word \033[3m\"%s\"\033[0m \033[0;32mfounded\033[0m\n\n", searching_word);
    return OK;
}

err_t del_act(word_tree_t **root) {
    if (!*root) return ERR_EMPTY_TREE;
    printf("Input first letter of deleting words: ");
    char letter, tmp;
    SCANF_CHECKER(2, "%c%c", &letter, &tmp);
    if (tmp != '\n') {
        clean_buf();
        return ERR_IO;
    }
    if (!isalpha(letter)) return ERR_IO;

    err_t err = OK;
    size_t count = delete_leafs(root, first_letter_cmp, &letter);
    if (count) printf("\033[0;32mSuccesfully\033[0m deleted %zu words beginnig on \033[3m\"%c\"\033[0m\n\n", count, letter);
    else printf("Words beginning on \033[3m\"%c\"\033[0m \033[0;31mnot founded\033[0m\n\n", letter);
    return err;
}
