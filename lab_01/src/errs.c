#include "../inc/errs.h"

#include <stdio.h>

void err_message(err_t err) {
    printf("\033[0;31mERROR: \033[0m");
    if (err == ERR_IO) printf("Incorrect input");
    else if (err == ERR_CALC_RANGE) printf("Calculated num out of acceptable values");
    else printf("Undefined");
    printf("\n");
}
