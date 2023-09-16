#include "../inc/errs.h"

#include <stdio.h>

void err_message(err_t err) {
    if (err == ERR_IO) printf("ERROR: Incorrect input");
    else if (err == ERR_CALC_RANGE) printf("ERROR: Calculated num out of acceptable values");
    else printf("ERROR: Undefined");
    printf("\n");
}
