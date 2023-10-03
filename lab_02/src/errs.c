#include "errs.h"

#include <stdio.h>

void err_print(err_t err) {
    if (err == ERR_IO) printf("\033[0;31mERROR:\033[0m Incorrect input\n\n");
    if (err == ERR_ACT) printf("\033[0;31mERROR:\033[0m Incorrect action\n\n");
    if (err == ERR_MEM) printf("\033[0;31mERROR:\033[0m Not enough memory\n\n");
    if (err == ERR_FILE) printf("\033[0;31mERROR:\033[0m Incorrect file\n\n");
}
