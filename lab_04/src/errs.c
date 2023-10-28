#include "errs.h"

#include <stdio.h>

void err_print(err_t err) {
    if (err == ERR_IO) printf("\033[0;31mERROR:\033[0m Incorrect input\n\n");
    if (err == ERR_ACT) printf("\033[0;31mERROR:\033[0m Incorrect action\n\n");
    if (err == ERR_CMD) printf("\033[0;31mERROR:\033[0m Incorrect choice\n\n");
    if (err == ERR_EXP) printf("\033[0;31mERROR:\033[0m Incorrect expression\n\n");
    if (err == ERR_OVERFLOW) printf("\033[0;31mERROR:\033[0m Stack buff overflow\n\n");
}
