#ifndef __ERRS_H__
#define __ERRS_H__

typedef enum {
    OK,
    ERR_IO,
    ERR_CMD,
    ERR_ACT,
    ERR_EXP,
    ERR_MEM,
    ERR_CHANCE,
    ERR_OVERFLOW,
} err_t;

#define SCANF_CHECKER(n, ...)                                                        \
    do {                                                                             \
        if (scanf(__VA_ARGS__) != n)                                                 \
            return ERR_IO;                                                           \
    } while (0)

#define ERR_GOTO(err_var, err_code, label)                                         \
    do {                                                                           \
        err_var = err_code;                                                        \
        goto label;                                                                \
    } while (0)

void err_print(err_t err);

#endif //__ERRS_H__
