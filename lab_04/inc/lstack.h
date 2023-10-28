#ifndef __LSTAK_H__
#define __LSTAK_H__

#include "errs.h"
#include <stddef.h>
#include <stdbool.h>

#include "stacklimits.h"

/// @typedef node_t -  Узел стека
typedef struct node {
    char el; // Элемент
    size_t index; // Индекс элемента в стеке
    struct node *prev; // Предыдущий элемент
} node_t;

/// @typedef lstack_t - Стек на основе односвязного списка
typedef struct {
    size_t capacity; // Вместимость стека
    node_t *top; // Вершина стека
} lstack_t;

/// @typedef free_adresses_t - Стек свободных адресов
typedef struct {
    size_t size; // Размер стека
    void *adresses[STACK_SIZE]; // Свободные адреса
} free_adresses_t;

extern free_adresses_t free_adresses;

void ls_init(lstack_t *stack);

err_t ls_push(lstack_t *s, char el);
char ls_pop(lstack_t *s);

char ls_peek(lstack_t *s);
bool ls_empty(lstack_t *s);

void ls_print(lstack_t *s);
void fadress_print(void);

void ls_free(lstack_t *s);

#endif // __LSTAK_H__
