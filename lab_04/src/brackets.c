#include "brackets.h"

#include "astack.h"
#include "lstack.h"
#include "stackmes.h"
#include "stacksproc.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef _WIN64
    #define _GNU_SOURCE
#else
    #include "helper.h"
#endif

#define BRACKETS_CNT 3

#define LEFT_BRACKETS "([{"
#define RIGHT_BRACKETS ")]}"

static const char *brackets[BRACKETS_CNT] = {"()", "{}", "[]"};

bool check_valid_by_as(const char *expr, err_t *err) {
    *err = OK;
    bool valid = true;
    astack_t left_brackets; as_init(&left_brackets);
    for (size_t i = 0; expr[i] != '\0' && valid; i++) {
        if (strchr(LEFT_BRACKETS, expr[i])) {
            *err = as_push(&left_brackets, expr[i]);
            if (*err) goto ERR;
        } else if (strchr(RIGHT_BRACKETS, expr[i])) {
            char right = expr[i];
            size_t ri = 0, li = 0;
            char left = as_pop(&left_brackets);
            for (size_t i = 0; i < BRACKETS_CNT; i++) {
                if (strchr(brackets[i], left)) li = i;
                if (strchr(brackets[i], right)) ri = i;
            }
            valid = (ri == li) ? true : false;
        }
    }
    valid = valid ? as_empty(&left_brackets) : valid;
    ERR: as_free(&left_brackets);
    return valid;
}

bool check_valid_by_ls(const char *expr, err_t *err) {
    *err = OK;
    bool valid = true;
    char *left, *right;
    lstack_t left_brackets; ls_init(&left_brackets);
    for (size_t i = 0; expr[i] != '\0' && valid; i++) {
        if ((left = strchr(LEFT_BRACKETS, expr[i]))) {
            *err = ls_push(&left_brackets, *left);
            if (*err) goto ERR;
        } else if ((right = strchr(RIGHT_BRACKETS, expr[i]))) {
            char right = expr[i];
            size_t ri = 0, li = 0;
            char left = ls_pop(&left_brackets);
            for (size_t i = 0; i < BRACKETS_CNT; i++) {
                if (strchr(brackets[i], left)) li = i;
                if (strchr(brackets[i], right)) ri = i;
            }
            valid = (ri == li) ? true : false;
        }
    }
    valid = valid ? ls_empty(&left_brackets) : valid;
    ERR: ls_free(&left_brackets);
    return valid;
}

err_t brackets_validate(void) {
    err_t err = OK;
    char *expr = NULL;
    printf("Input expression: ");
#ifndef _WIN64
    size_t len = 0;
    if (getline(&expr, &len, stdin) <= 1)
        ERR_GOTO(err, ERR_EXP, fexp);
#else
    if (!(expr = get_str(stdin, '\n', -1)))
        ERR_GOTO(err, ERR_EXP, fexp);
#endif
    char *stack_type_inf[STACKS_CNT] = {
        "\n\033[0;34mARRAY STACK:\033[0m", 
        "\n\033[0;34mLIST STACK:\033[0m"
    };
    for (size_t i = 0; i < STACKS_CNT; i++) {
        ssize_t beg = microseconds_now();
        bool is_valid = !(i & 1) ? check_valid_by_as(expr, &err) : check_valid_by_ls(expr, &err);
        ssize_t end = microseconds_now();
        
        if (err) goto fexp;
        printf(
            "%s [%zd mcs] Brackets are %s", 
            stack_type_inf[i], end - beg, 
            is_valid ? "\033[0;32mvalid\033[0m" : "\033[0;31minvalid\033[0m"
        );
    }
    printf("\n\n");
    fexp: free(expr);
    return err;
}
