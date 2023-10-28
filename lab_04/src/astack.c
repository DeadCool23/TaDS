#include "astack.h"

#include <stdio.h>
#include <stdlib.h>

void as_init(astack_t *stack) {
    stack->capacity = STACK_SIZE;
    stack->el = stack->stack - 1;
}

err_t as_push(astack_t *s, char el) {
    if (s->el == s->stack + s->capacity - 1)
        return ERR_OVERFLOW;
    ++s->el; *s->el = el;
    return OK;
}

char as_pop(astack_t *s) {
    if (s->el == s->stack - 1)
        return '\0';
    return *s->el--;
}

char as_peek(astack_t *s) { 
    return (s->el == s->stack - 1) ? *s->el : '\0'; 
}

bool as_empty(astack_t *s) { return s->el == s->stack - 1; }

void as_print(astack_t *s) {
    if (s->el == s->stack - 1) return;
    printf("|%4c|%p|\n", *s->el, (void *)s->el);
    s->el--; as_print(s); s->el++;
}

void as_free(astack_t *s) { s->el = s->stack - 1; }
