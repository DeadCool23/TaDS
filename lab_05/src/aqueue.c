#include "aqueue.h"

#include <stdio.h>
#include <stdlib.h>

void aq_init(aqueue_t *queue) { 
    queue->els_cnt = 0;
    queue->first = queue->last = queue->queue;
}

err_t aq_push(aqueue_t *q, double el) {
    if (q->els_cnt == QUEUE_SIZE)
        return ERR_OVERFLOW;
    
    if (q->last == q->queue + QUEUE_SIZE)
        q->last = q->queue;
    
    *q->last++ = el;
    q->els_cnt++;
    return OK;
}

double aq_pop(aqueue_t *q) {
    if (!q->els_cnt)
        return -1;

    double el = *q->first++;
    if (q->first == q->queue + QUEUE_SIZE)
        q->first = q->queue;
    
    q->els_cnt--;

    return el;
}

double aq_peek(aqueue_t *q) { return (!q->els_cnt) ? *q->first : '\0'; }

bool aq_empty(aqueue_t *q) { return q->last == q->first; }

size_t aq_len(aqueue_t *q) { return q->els_cnt; }

void aq_free(aqueue_t *q) { 
    q->els_cnt = 0;
    q->first = q->last = q->queue;
}
