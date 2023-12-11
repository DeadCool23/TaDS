#include "action.h"

#include "menu.h"
#include "helper.h"
#include "fileprocess.h"

#include <ctype.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

static void free_filename(char **filename) {
    free(*filename);
    *filename = NULL;
}

err_t load_act(word_tree_t **root, word_tree_t **avl_root, ohash_table_t *otable, chash_table_t *ctable, char **filename) {
    if (*root) tree_destroy(*root);
    if (*avl_root) tree_destroy(*avl_root);
    if (ctable->size) {
        chash_free(ctable);
        chash_init(ctable, BEG_SIZE);
    }
    if (otable->size) {
        ohash_free(otable);
        ohash_init(otable, BEG_SIZE);
    }

    *root = NULL; *avl_root = NULL;

    free_filename(filename);

    int type;
    data_menu();
    SCANF_CHECKER(1, "%d", &type);
    clean_buf();
    err_t err = prepare_file(filename, type - 1);
    err = !err ? read_tree_from_file(root, *filename) : err;
    if (err) goto end;
    printf("\n\n");
    printf(!err ? "Data successfully loaded to tree\n" : "Failed loading data to tree\n");
    printf("---\n");
    *avl_root = !err ? copy_tree_with_balance(*avl_root, *root) : NULL;
    word_tree_t *tmp = NULL; copy_tree_with_balance(tmp, *avl_root);
    if (tmp) {
        tree_destroy(*avl_root);
        *avl_root = tmp;
    }
    err = !avl_root ? ERR_MEM : err;
    printf(!err ? "Data successfully loaded to avl tree\n" : "Failed loading data to avl tree\n");
    printf("---\n");
    err = !err ? read_chash_from_file(ctable, *filename, true) : err;
    printf("\n");
    printf(!err ? "Data successfully loaded to closed hash table\n" : "Failed loading data to closed hash table\n");
    printf("---\n");
    err = !err ? read_ohash_from_file(otable, *filename, true) : err;
    printf("\n");
    printf(!err ? "Data successfully loaded to opened hash table\n" : "Failed loading data to opened hash table\n");
    printf("---\n");
    if (err) free_filename(filename);
    else printf("\n\nData \033[0;32msuccessfully\033[0m loaded\n\n");
    end: return err;
}

err_t add_act(word_tree_t **root, word_tree_t **avl_root, ohash_table_t *otable, chash_table_t *ctable) {
    add_menu();
    int act = 0;
    SCANF_CHECKER(1, "%d", &act);
    clean_buf();

    err_t err = OK;
    string adding_word;
    printf("Input adding word: ");
    err = word_read(stdin, &adding_word);    
    if (err) goto end;

    switch (act) {
        case 7:
        case 5:
        case 3:
            ohash_add(otable, adding_word, true);
            if (act == 3) break;
        case 4:
            chash_add(ctable, adding_word, true);
            if (act == 4 || act == 5) break;
        case 6:
        case 1:
            *root = add_leaf(*root, leaf_create(adding_word));
            if (act == 1) break;
        case 2:
            *avl_root = add_leaf_with_balance(*avl_root, leaf_create(adding_word));
            break;
        default:
            err = ERR_CMD;
            break;
    }

    printf("Word \033[3m\"%s\"\033[0m \033[0;32msuccessfully\033[0m added\n\n", adding_word);
    end: free(adding_word);
    return err;
}

err_t find_act(word_tree_t *root) {
    if (!root) return ERR_EMPTY_TREE;

    err_t err = OK;
    string searching_word;
    printf("Input searching word: ");
    err = word_read(stdin, &searching_word);    
    if (err) goto end;

    word_tree_t *leaf = find_leaf(root, words_cmp, searching_word, NULL);
    if (!leaf) printf("Word \033[3m\"%s\"\033[0m \033[0;31mnot founded\033[0m\n\n", searching_word);
    else printf("Word \033[3m\"%s\"\033[0m \033[0;32mfounded\033[0m\n\n", searching_word);
    end: free(searching_word);
    return err;
}

err_t del_tree_act(word_tree_t **root, word_tree_t **avl_root) {
    if (!*root || !*avl_root) return ERR_EMPTY_TREE;
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
    (void)delete_leafs(avl_root, first_letter_cmp, &letter);
    word_tree_t *tmpp = NULL; tmpp = copy_tree_with_balance(tmpp, *avl_root);
    tree_destroy(*avl_root); *avl_root = tmpp;
    if (count) printf("\033[0;32mSuccesfully\033[0m deleted %zu words beginnig on \033[3m\"%c\"\033[0m\n\n", count, letter);
    else printf("Words beginning on \033[3m\"%c\"\033[0m \033[0;31mnot founded\033[0m\n\n", letter);
    return err;
}

err_t del_hash_act(chash_table_t *ctable, ohash_table_t *otable) {
    err_t err = OK;
    string searching_word;
    printf("Input deliting word: ");
    err = word_read(stdin, &searching_word);    
    if (err) goto end;

    int is_finded = 0;
    is_finded += chash_remove(ctable, searching_word, NULL) ? 1 : 0;
    is_finded += ohash_remove(otable, searching_word, NULL) ? 1 : 0;
    printf(is_finded ? "Word \033[3m\"%s\"\033[0m \033[0;32msuccessfully\033[0m deleted\n\n" 
    : "Word \033[3m\"%s\"\033[0m \033[0;31mnot founded\033[0m\n\n", searching_word);
    end: free(searching_word);
    return err;
}
