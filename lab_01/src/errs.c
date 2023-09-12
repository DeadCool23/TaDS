#include "errs.h"

#include <stdio.h>

void err_message(err_t err) {
    if (err == ERR_IO) printf("ERROR: Incorrect input");
    else if (err == ERR_RANGE) printf("ERROR: Incorrect range");
    else if (err == ERR_CALC_RANGE) printf("ERROR: Calculated num too long");
    else printf("ERROR: Undefined");
    printf("\n");
}