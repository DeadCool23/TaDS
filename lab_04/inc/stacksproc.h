#ifndef __STACKSPROC_H__
#define __STACKSPROC_H__

#include "errs.h"

#include "astack.h"
#include "lstack.h"

#define STACKS_CNT 2

typedef enum {
    ARR_STACK,
    LIST_STACK
} stack_type_t;

err_t poper(astack_t *as, lstack_t *ls);
err_t pusher(astack_t *as, lstack_t *ls);
err_t printer(astack_t *as, lstack_t *ls);

#endif // __STACKSPROC_H__
