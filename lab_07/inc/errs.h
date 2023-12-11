#ifndef __ERRS_H__
#define __ERRS_H__

#include <stdio.h>

typedef enum {
    OK,                // Все хорошо
    ERR_IO,            // Ошибка чтения
    ERR_CMD,           // Ошибка команды
    ERR_ACT,           // Ошибка акта
    ERR_MEM,           // Ошибка памяти
    ERR_FILE,          // Ошибка файла
    ERR_EMPTY_FILE,    // Ошибка пустого файла
    ERR_EMPTY_TREE,    // Ошибка пустого дерева
} err_t;

#define SCANF_CHECKER(n, ...)                                                        \
    do {                                                                             \
        if (scanf(__VA_ARGS__) != n)                                                 \
            return ERR_IO;                                                           \
    } while (0)

#define ERR_GOTO(err_var, err_code, label)                                           \
    do {                                                                             \
        err_var = err_code;                                                          \
        goto label;                                                                  \
    } while (0)

void err_print(err_t err);

#endif //__ERRS_H__
