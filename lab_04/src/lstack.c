#include "lstack.h"

#include <stdio.h>
#include <stdlib.h>

free_adresses_t free_adresses = {
    .size = 0,
    .adresses = { NULL },
};

void ls_init(lstack_t *stack) {
    stack->capacity = STACK_SIZE;
    stack->top = NULL;
}

err_t ls_push(lstack_t *s, char el) {
    node_t *node = (!free_adresses.size) 
        ? malloc(sizeof(*node)) 
        : free_adresses.adresses[--free_adresses.size];
    node->index = (!s->top) ? 0 : s->top->index + 1;
    if (node->index > s->capacity - 1) return ERR_OVERFLOW;
    node->el = el; node->prev = s->top;
    s->top = node;
    return OK;
}

char ls_pop(lstack_t *s) {
    if (!s->top) return '\0';
    char el = s->top->el;
    free_adresses.adresses[free_adresses.size++] = s->top;
    s->top = s->top->prev;
    return el;
}

char ls_peek(lstack_t *s) {
    return (s->top) ? s->top->el : '\0';
}

bool ls_empty(lstack_t *s) { return s->top == NULL; }

void ls_print(lstack_t *s) {
    if (!s->top) return;
    printf("|%4c|%p|\n", s->top->el, (void *)s->top);
    node_t *node = s->top;
    s->top = s->top->prev; ls_print(s); s->top = node;
}

void fadress_print(void) {
    if (!free_adresses.size) return;
    printf("|%p|\n", free_adresses.adresses[free_adresses.size - 1]);
    free_adresses.size--; fadress_print(); free_adresses.size++;
}

static void ffadresses(void) { while (free_adresses.size) free(free_adresses.adresses[--free_adresses.size]); }

void ls_free(lstack_t *s) {
    if (!s->top) {
        ffadresses();
        s->top = NULL;
        return;
    }
    node_t *top = s->top;
    free(top);
    s->top = s->top->prev;
    ls_free(s);
}
