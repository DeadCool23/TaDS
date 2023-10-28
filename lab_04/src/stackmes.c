#include "stackmes.h"

#include "astack.h"
#include "lstack.h"

#include "brackets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
    size_t time;
    size_t mem;
} mes_t;

#define ACTS_CNT 3
#define STACKS_CNT 2

typedef enum {
    ARR_STACK,
    LIST_STACK
} stack_type_t;

#define UNDER_SIZE "-----"
#define UNDER_ACT "---------"
#define UNDER_MEASURES "---------"
#define UNDER_ARR_STACK "-------------"
#define UNDER_LIST_STACK "-------------"

#define ACT_FORMAT_LEN 9
#define SIZE_FORMAT_LEN 5

#define TABLE_SIZE 55

#define BETWEEN_ACTS printf("+%-9s+%5s+%-9s+%13s+%13s+\n", \
                             UNDER_ACT, UNDER_SIZE, UNDER_MEASURES, \
                             UNDER_ARR_STACK, UNDER_LIST_STACK)

#define BETWEEN_SIZES printf("|%-9s+%5s+%-9s+%13s+%13s+\n", \
                             " ", UNDER_SIZE, UNDER_MEASURES, \
                             UNDER_ARR_STACK, UNDER_LIST_STACK)

ssize_t microseconds_now(void) {
    struct timeval val;

    if (gettimeofday(&val, 0))
        return (ssize_t) -1;

    return val.tv_sec * 1000ULL * 1000ULL + val.tv_usec;
}

/*
|         | Size| Measures| Arr stack  | List stack   |
+---------+-----+---------+------------+--------------+
|      Pop|   10|Time, mcs|            |              |
|         |     |   Mem, b|            |              |
|         +-----+---------+------------+--------------+
|         |  100|Time, mcs|            |              |
|         |     |   Mem, b|            |              |
|         +-----+---------+------------+--------------+
|         | 1000|Time, mcs|            |              |
|         |     |   Mem, b|            |              |
+---------+-----+---------+------------+--------------+
*/      

static void liner(void) { 
    for (size_t i = 0; i < TABLE_SIZE; i++) 
        printf((i == 0 || i == TABLE_SIZE - 1) ? "+" : "-");
    printf("\n");
}

static void header(void) {
    liner();
    printf("|\033[0;34m%-9s\033[0m|\033[0;34m%-5s\033[0m|\033[0;34m%-9s\033[0m|\033[0;34m%-13s\033[0m|\033[0;34m%-13s\033[0m|\n",
    " ", "Size", "Measures", "Arr stack", "List stack");
    BETWEEN_ACTS;
}

static void get_mem(size_t size, mes_t *mes) {
    astack_t tmp_s;
    mes[ARR_STACK].mem = sizeof(tmp_s.capacity) + sizeof(tmp_s.el) + sizeof(*tmp_s.stack) * size;
    mes[LIST_STACK].mem = sizeof(lstack_t) + size * sizeof(node_t);
}

static void get_pop_time(size_t size, astack_t *as, lstack_t *ls, mes_t *mes) {
    size_t beg, end;
    mes[ARR_STACK].time = mes[LIST_STACK].time = 0;
    for (size_t type = ARR_STACK; type < STACKS_CNT; type++) {
        for (size_t i = 0; i < size; i++) {
            beg = microseconds_now();
            char elem = !(type & 1) ? as_pop(as) : ls_pop(ls); (void)elem;
            end = microseconds_now();
            mes[!(type & 1) ? ARR_STACK : LIST_STACK].time += end - beg;
        }
    }
}

static void get_push_time(size_t size, astack_t *as, lstack_t *ls, mes_t *mes) {
    char elem = '0'; 
    size_t beg, end;
    mes[ARR_STACK].time = mes[LIST_STACK].time = 0;
    for (size_t type = ARR_STACK; type < STACKS_CNT; type++) {
        for (size_t i = 0; i < size; i++) {
            beg = microseconds_now();
            !(type & 1) ? as_push(as, elem) : ls_push(ls, elem);
            end = microseconds_now();
            mes[!(type & 1) ? ARR_STACK : LIST_STACK].time += end - beg;
        }
    }
}

static char *gen_expr(size_t size) {
    char *expr = malloc((size + 1) * sizeof(*expr));
    if (!expr) return NULL;
    strcpy(expr, "\0");
    for (size_t i = 0; i < size / 2; i++) strcat(expr, "()");
    return expr;
}

static void get_brackets_time(size_t size, mes_t *mes) {
    size_t beg, end;
    char *expr = gen_expr(size);
    mes[ARR_STACK].time = mes[LIST_STACK].time = 0;
    for (size_t type = ARR_STACK; type < STACKS_CNT; type++) {
        err_t err = OK;
        beg = microseconds_now();
        !(type & 1) ? check_valid_by_as(expr, &err) : check_valid_by_ls(expr, &err);
        end = microseconds_now();
        mes[!(type & 1) ? ARR_STACK : LIST_STACK].time += end - beg;
    }
    free(expr);
}

void get_mes(void) {
    header();
    mes_t mes[STACKS_CNT];
    astack_t as; as_init(&as);
    lstack_t ls; ls_init(&ls);
    char *acts[ACTS_CNT] = {"Push", "Pop", "Brackets"};
    for (size_t act = 0; act < ACTS_CNT; act++) {
        for (size_t size = 10; size <= 1000; size *= 10) {
            get_mem(size, mes);
            switch (act) {
                case 0:
                    get_push_time(size, &as, &ls, mes);
                    break;
                case 1:
                    get_pop_time(size, &as, &ls, mes);
                    break;
                case 2:
                    get_brackets_time(size, mes);
                    break;
            }
            (size == 10) 
            ? printf("|%-9s|%-5zu|%-9s|%-13zu|%-13zu|\n", acts[act], size, "Time, mcs", mes[ARR_STACK].time, mes[LIST_STACK].time)
            : printf("|%-9s|%-5zu|%-9s|%-13zu|%-13zu|\n", " ", size, "Time, mcs", mes[ARR_STACK].time, mes[LIST_STACK].time);
            printf("|%-9s|%-5s|%-9s|%-13zu|%-13zu|\n", " ", " ", "Mem, b", mes[ARR_STACK].mem, mes[LIST_STACK].mem);
            if (size != 1000) BETWEEN_SIZES;
        }
        if (act + 1 != ACTS_CNT) BETWEEN_ACTS;
    }
    as_free(&as); ls_free(&ls);
    liner();
}
