#ifndef __ASTAK_H__
#define __ASTAK_H__

#include "errs.h"
#include <stddef.h>
#include <stdbool.h>

#include "stacklimits.h"

/// @typedef astak_t - Стек на основе статического массива
typedef struct {
    size_t capacity; // Вместимость стека
    char stack[STACK_SIZE]; // Массив елементов стека
    char *el; // Вершина стека
} astack_t;

void as_init(astack_t *stack);

err_t as_push(astack_t *s, char el);
char as_pop(astack_t *s);

char as_peek(astack_t *s);
bool as_empty(astack_t *s);

void as_print(astack_t *s);

void as_free(astack_t *s);

#endif // __ASTAK_H__
