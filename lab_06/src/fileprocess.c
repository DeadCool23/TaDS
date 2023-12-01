#include "fileprocess.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "errs.h"
#include "helper.h"

static const char *tmp_filename = "/tmp/tree.txt";
static const char *gen_filename = "./misc/gen/generated.txt";
static const char *pygen_filename = "./misc/gen/gen.py";

err_t word_read(FILE *file, string *word) {
    *word = NULL;
    ssize_t read;
    err_t err = OK;
    size_t len = 0;
    if ((read = getline(word, &len, file)) == -1)
        err = ERR_IO;
    
    if (!err)
    {
        char *word_end = strchr(*word, '\n');
        if (word_end)
            *word_end = '\0';
        
        err = !strchr(*word, '\0') ? ERR_IO : err;
        err = !strlen(*word) ? ERR_IO : err;
    }
    return err;
}

/**
 * @brief Запись слова в файл
 * 
 * @param [in, out] file - файловая переменная
 * @param [in] word - записываемое слово
 */
static void word_write(FILE *file, const string word) { fprintf(file, "%s\n", word); }

err_t read_tree_from_file(word_tree_t **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return ERR_FILE;

    err_t err = OK;
    string word = NULL;
    word_tree_t *new_leaf = NULL;
    for (;!err && !is_eof(file);) {
        err = word_read(file, &word);
        if (!err) {
            new_leaf = leaf_create(word);
            err = !err ? new_leaf ? err : ERR_MEM : err;
            *root = new_leaf ? add_leaf(*root, new_leaf) : *root;
        }
        free(word);
    }
    fclose(file);
    return !err ? *root ? OK : ERR_EMPTY_FILE : err;
}

static err_t generate_file(void) {
#define MAX_PYTHON_CALL 1000
    int str_cnt = 0;
    printf("Input count of strings in file\n[Range: 0 < x <= 2000] : ");
    SCANF_CHECKER(1, "%d", &str_cnt);
    if (str_cnt <= 0 || str_cnt > 2000) return ERR_IO;

    char python_call[MAX_PYTHON_CALL];
    strcpy(python_call, "\0");
    sprintf(python_call, "python3 %s %d", pygen_filename, str_cnt);

    system(python_call);
    return OK;
#undef MAX_PYTHON_CALL
}

err_t prepare_file(char **filename, tree_load_t type) {
    err_t err = OK;
    switch (type) {
        case GENERATE:
            *filename = strdup(gen_filename);
            if (*filename == NULL) return ERR_MEM;
            err = generate_file();
            break;
        case LOAD:
            printf("Input filename: ");
            err = word_read(stdin, filename);
            break;
        default:
            return ERR_CMD;
    }
    return err;
}

err_t del_from_file(const char *filename, uni_cmp_t cmp, void *param) {
    err_t err = OK;
    FILE *file = fopen(filename, "r");
    if (!file) return ERR_FILE;
    if (is_eof(file)) ERR_GOTO(err, ERR_EMPTY_FILE, close);
    FILE *tmp_file = fopen(tmp_filename, "w");
    if (!tmp_file) ERR_GOTO(err, ERR_FILE, close);

    string word = NULL;
    while (!is_eof(file) && !err) {
        err = word_read(file, &word);
        if (!cmp(word, param))
            word_write(tmp_file, word);
        free(word);
    }

    fclose(tmp_file);
    close: fclose(file);

    if (err == OK) {
        if (remove(filename) != 0)
            err = ERR_IO;
        else if (rename(tmp_filename, filename) != 0)
            err = ERR_IO;
    }

    return err;
}
