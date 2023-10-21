#ifndef __MULT_MES__
#define __MULT_MES__

#include "errs.h"

#include <stddef.h>
#include <unistd.h>

typedef struct {
    size_t time;
    size_t mem;
} mes_t;

ssize_t microseconds_now(void);
err_t mes_table(void);

#endif // __MULT_MES__
