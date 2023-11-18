#include "lqueue.h"

#include <stdio.h>
#include <stdlib.h>

#include "queuelimits.h"

lqueue_t *lq_init(void) { return NULL; }

err_t lq_push(lqueue_t **q, double el) {
    size_t cnt = 0;
    lqueue_t *inode = NULL, *prev = NULL;
    for (inode = *q; inode != NULL; inode = inode->next, cnt++)
        prev = inode;    
    if (cnt == QUEUE_SIZE) return ERR_OVERFLOW;
    
    lqueue_t *add_node = NULL;
    add_node = malloc(sizeof(*add_node));
    if (!add_node) return ERR_MEM;
    add_node->el = el; add_node->next = NULL;

    if (!prev)
        *q = add_node;
    else
        prev->next = add_node;

    inode = add_node;
    return OK;
}

double lq_pop(lqueue_t **q) {
    if (!*q) return -1;

    double el = (*q)->el;
    lqueue_t *node = (*q)->next;

    free(*q);
    *q = node;

    return el;
}

double lq_peek(lqueue_t *q) { return (q) ? q->el : '\0'; }

bool lq_empty(lqueue_t *q) { return q->next == NULL; }

size_t lq_len(lqueue_t *q) {
    size_t cnt = 0;
    for (lqueue_t *inode = q; inode != NULL; inode = inode->next, cnt++);
    return cnt;
}

void lq_free(lqueue_t *q) {
    if (!q) return;

    lq_free(q->next);
    free(q);
}
