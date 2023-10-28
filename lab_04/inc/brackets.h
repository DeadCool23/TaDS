#ifndef __BRACKETS_H__
#define __BRACKETS_H__

#include "errs.h"

#include <stdbool.h>

bool check_valid_by_as(const char *expr, err_t *err);
bool check_valid_by_ls(const char *expr, err_t *err);

err_t brackets_validate(void);

#endif // __BRACKETS_H__
