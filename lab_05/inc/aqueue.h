#ifndef __AQUEUE_H__
#define __AQUEUE_H__

#include "errs.h"
#include <stddef.h>
#include <stdbool.h>

#include "queuelimits.h"

/// @typedef aqtak_t - Очередь на основе статического массива
typedef struct {
    double queue[QUEUE_SIZE];      // Массив елементов очереди
    size_t els_cnt;                // Кол-во элементов в очереди
    double *first;                 // Первый в очереди
    double *last;                  // Последний в очереди
} aqueue_t;

void aq_init(aqueue_t *queue);

err_t aq_push(aqueue_t *q, double el);
double aq_pop(aqueue_t *q);

double aq_peek(aqueue_t *q);
bool aq_empty(aqueue_t *q);

size_t aq_len(aqueue_t *q);

void aq_free(aqueue_t *q);

#endif // __AQUEUE_H__
