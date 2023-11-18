#ifndef __MES_H__
#define __MES_H__

#include <stdio.h>

typedef struct {
    size_t mem;
    ssize_t time;
} mes_t;

void print_cmp(const mes_t *list, const mes_t *arr);

#endif // __MES_H__
