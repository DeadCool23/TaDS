#include "mes.h"
#include "fileprocess.h"
#include "tree.h"
#include <stdlib.h>

typedef struct {
    size_t time;
    size_t mem;
} mes_t;

#define MES_CNT 1000
#define DEL_LETTER 'm'
#define MES_TYPES_CNT 2

typedef enum {
    TREE_MES,
    FILE_MES
} mes_types_t;

static const char *tmp_filename = "./misc/measure/tmp.txt";

static const char *linear_filename = "./misc/measure/linear.txt";
static const char *bigdata_filename = "./misc/measure/bigdata.txt";
static const char *nodelwords_filename = "./misc/measure/nodelwords.txt";

static size_t get_ticks(void) {
    size_t ticks;
    __asm__ __volatile__ ("rdtsc": "=A" (ticks));
    return ticks;
}

#define CMD_SIZE 1024

static void create_tmp(const char *filename, const char *tmp_filename) {
    char cmd[CMD_SIZE];
    sprintf(cmd, "cp %s %s", filename, tmp_filename);
    system(cmd);
}

static void delete_file(const char *filename) {
    char cmd[CMD_SIZE];
    sprintf(cmd, "rm -fr %s", filename);
    system(cmd);
}

static err_t get_mes(const char *filename, mes_t *mes) {
    err_t err = OK;
    word_tree_t *tree;
    char del_letter = DEL_LETTER;
    mes[TREE_MES] = mes[FILE_MES] = (mes_t) { .time = 0, .mem = 0 };

    for (size_t i = 0; i < MES_CNT && !err; i++) {
        tree = tree_init();
        create_tmp(filename, tmp_filename);
        err = read_tree_from_file(&tree, tmp_filename);
        if (err) goto end;

        /* Получение затраченной памяти */
        if (!mes[FILE_MES].mem || !mes[TREE_MES].mem) {
            size_t words_cnt = leafs_cnt(tree);
            mes[FILE_MES].mem = words_cnt * sizeof(string);
            mes[TREE_MES].mem = words_cnt * sizeof(*tree);
        }

        /* Замер удаления из файла */
        size_t fbeg = get_ticks();
        err = del_from_file(tmp_filename, first_letter_cmp, &del_letter);
        size_t fend = get_ticks();
        mes[FILE_MES].time += fend - fbeg;

        if (err) goto end;

        /* Замер удаления из дерева */
        size_t tbeg = get_ticks();
        (void)delete_leafs(&tree, first_letter_cmp, &del_letter);
        size_t tend = get_ticks();
        mes[TREE_MES].time += tend - tbeg;

        tree_destroy(tree);
    }

    mes[FILE_MES].time /= MES_CNT;
    mes[TREE_MES].time /= MES_CNT;

    delete_file(tmp_filename);
    end: return err;
}

#undef CMD_SIZE

#define LINEAR printf("+-------------+-------------------------+-------------------------+\n");

static void print_mes(mes_t *mes) {
    printf("| Time, ticks | %23zu | %23zu |\n", mes[TREE_MES].time, mes[FILE_MES].time);
    printf("| Memory, b   | %23zu | %23zu |\n", mes[TREE_MES].mem, mes[FILE_MES].mem);
}

err_t cmp_del_from_file_and_tree(void) {
    err_t err = OK;
    mes_t mes[MES_TYPES_CNT];
    LINEAR;
    printf("|             |          Tree           |           File          |\n");

    LINEAR;
    printf("| \033[0;33mBig data\033[0m                                                        |\n");
    LINEAR;

    err = get_mes(bigdata_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("| \033[0;33mLinear\033[0m                                                          |\n");
    LINEAR;

    err = get_mes(linear_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("| \033[0;33mNo words for delete\033[0m                                             |\n");
    LINEAR;

    err = get_mes(nodelwords_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("\n\n");

    end: return err;
}
