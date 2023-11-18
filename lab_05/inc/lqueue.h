#ifndef __LQUEUE_H__
#define __LQUEUE_H__

#include "errs.h"
#include <stddef.h>
#include <stdbool.h>

/// @typedef node_t -  Очередь на основе списка
typedef struct node {
    double el;              // Элемент
    struct node *next;      // Следующий элемент
} lqueue_t;

lqueue_t *lq_init(void);

err_t lq_push(lqueue_t **q, double el);
double lq_pop(lqueue_t **q);

double lq_peek(lqueue_t *q);
bool lq_empty(lqueue_t *q);
size_t lq_len(lqueue_t *q);

void lq_free(lqueue_t *q);

#endif // __LQUEUE_H__
