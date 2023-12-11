#include "mes.h"
#include "dot.h"
#include "tree.h"
#include "hash.h"
#include "fileprocess.h"

#include <stdlib.h>

typedef struct {
    size_t time;
    size_t mem;
    size_t cmp_cnt;
} mes_t;

#define MES_CNT 1000
#define DEL_WORD "zzzu"
#define MES_TYPES_CNT 5

typedef enum {
    BTREE_MES,
    AVLTREE_MES,
    OHASHTABLE_MES,
    CHASHTABLE_MES,
    FILE_MES,
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
    word_tree_t *tree = NULL;
    word_tree_t *avl_tree = NULL;
    ohash_table_t otable; chash_table_t ctable;
    
    string del_word = DEL_WORD;
    mes[FILE_MES]
    = mes[BTREE_MES] = mes[AVLTREE_MES]
    = mes[OHASHTABLE_MES] = mes[CHASHTABLE_MES] 
    = (mes_t) { .time = 0, .mem = 0, .cmp_cnt = 0 };

    for (size_t i = 0; i < MES_CNT && !err; i++) {
        tree = tree_init();
        avl_tree = tree_init();
        word_tree_t *del_leaf = NULL;
        chash_init(&ctable, BEG_SIZE);
        ohash_init(&otable, BEG_SIZE);
        create_tmp(filename, tmp_filename);
        err = read_tree_from_file(&tree, tmp_filename);
        avl_tree = copy_tree_with_balance(avl_tree, tree);
        read_chash_from_file(&ctable, tmp_filename, false);
        read_ohash_from_file(&otable, tmp_filename, false);
        err = avl_tree ? err : ERR_MEM;
        if (err) goto end;

        /* Получение затраченной памяти */
        if (!mes[FILE_MES].mem || !mes[BTREE_MES].mem) {
            size_t words_cnt = leafs_cnt(tree);
            mes[FILE_MES].mem = words_cnt * sizeof(string);
            mes[BTREE_MES].mem = words_cnt * sizeof(*tree);
            mes[AVLTREE_MES].mem = words_cnt * sizeof(*avl_tree);
            mes[CHASHTABLE_MES].mem = words_cnt * sizeof(*ctable.hash_table);
            mes[OHASHTABLE_MES].mem = words_cnt * sizeof(**otable.hash_table);
        }

        /* Замер удаления из файла */
        size_t fbeg = get_ticks();
        err = del_from_file(tmp_filename, words_cmp, del_word, mes[FILE_MES].cmp_cnt ? NULL : &mes[FILE_MES].cmp_cnt);
        size_t fend = get_ticks();
        mes[FILE_MES].time += fend - fbeg;

        if (err) goto end;

        /* Замер удаления из обычного дерева */
        size_t btbeg = get_ticks();
        del_leaf = find_leaf(tree, words_cmp, del_word, mes[BTREE_MES].cmp_cnt ? NULL : &mes[BTREE_MES].cmp_cnt);
        tree = delete_leaf(tree, del_leaf);
        size_t btend = get_ticks();
        mes[BTREE_MES].time += btend - btbeg;

        /* Замер удаления из AVL дерева */
        size_t avltbeg = get_ticks();
        del_leaf = find_leaf(avl_tree, words_cmp, del_word, mes[AVLTREE_MES].cmp_cnt ? NULL : &mes[AVLTREE_MES].cmp_cnt);
        avl_tree = delete_leaf(avl_tree, del_leaf);
        size_t avltend = get_ticks();
        mes[AVLTREE_MES].time += avltend - avltbeg;

        /* Замер удаления из открытой хэш таблицы */
        size_t ohbeg = get_ticks();
        ohash_remove(&otable, del_word, mes[OHASHTABLE_MES].cmp_cnt ? NULL : &mes[OHASHTABLE_MES].cmp_cnt);
        size_t ohend = get_ticks();
        mes[AVLTREE_MES].time += ohend - ohbeg;

        /* Замер удаления из закрытой хэш таблицы */
        size_t chbeg = get_ticks();
        chash_remove(&ctable, del_word, mes[CHASHTABLE_MES].cmp_cnt ? NULL : &mes[CHASHTABLE_MES].cmp_cnt);
        size_t chend = get_ticks();
        mes[AVLTREE_MES].time += chend - chbeg;

        tree_destroy(tree);
        chash_free(&ctable);
        ohash_free(&otable);
    }

    mes[FILE_MES].time /= MES_CNT;
    mes[BTREE_MES].time /= MES_CNT;
    mes[AVLTREE_MES].time /= MES_CNT;
    mes[OHASHTABLE_MES].time /= MES_CNT;
    mes[CHASHTABLE_MES].time /= MES_CNT;

    delete_file(tmp_filename);
    end: return err;
}

#undef CMD_SIZE



#define LINEAR printf("+-------------+--------------+--------------+--------------+--------------+--------------+\n");
#define HEADER printf("|             |   Bin Tree   |   Avl Tree   |Hash Table (O)|Hash Table (C)|     File     |\n");

static void print_mes(mes_t *mes) {
    printf("| Time, ticks | %12zu | %12zu | %12zu | %12zu | %12zu |\n", 
    mes[BTREE_MES].time, mes[AVLTREE_MES].time, mes[OHASHTABLE_MES].time, mes[CHASHTABLE_MES].time, mes[FILE_MES].time);
    printf("| Memory, b   | %12zu | %12zu | %12zu | %12zu | %12zu |\n", 
    mes[BTREE_MES].mem, mes[AVLTREE_MES].mem, mes[OHASHTABLE_MES].mem, mes[CHASHTABLE_MES].mem, mes[FILE_MES].mem);
    printf("| Cmp count   | %12zu | %12zu | %12zu | %12zu | %12zu |\n", 
    mes[BTREE_MES].cmp_cnt, mes[AVLTREE_MES].cmp_cnt, mes[OHASHTABLE_MES].cmp_cnt, mes[CHASHTABLE_MES].cmp_cnt, mes[FILE_MES].cmp_cnt);
}

err_t cmp_del_from_file_and_trees_and_hash_tables(void) {
    err_t err = OK;
    mes_t mes[MES_TYPES_CNT];

    LINEAR;
    printf("|                                       \033[0;33mBig data\033[0m                                         |\n");
    LINEAR;

    HEADER;
    LINEAR;

    err = get_mes(bigdata_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("|                                        \033[0;33mLinear\033[0m                                          |\n");
    LINEAR;

    HEADER;
    LINEAR;

    err = get_mes(linear_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("|                                  \033[0;33mNo words for delete\033[0m                                   |\n");
    LINEAR;

    HEADER;
    LINEAR;

    err = get_mes(nodelwords_filename, mes);
    if (err) goto end;
    print_mes(mes);

    LINEAR;
    printf("\n\n");

    end: return err;
}
