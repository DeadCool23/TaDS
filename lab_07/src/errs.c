#include "errs.h"

#include <stdio.h>

void err_print(err_t err) {
    if (err == ERR_IO) printf("\033[0;31mERROR:\033[0m Incorrect input\n\n");
    if (err == ERR_ACT) printf("\033[0;31mERROR:\033[0m Incorrect action\n\n");
    if (err == ERR_CMD) printf("\033[0;31mERROR:\033[0m Incorrect choice\n\n");
    if (err == ERR_MEM) printf("\033[0;31mERROR:\033[0m Can't allocate memory\n\n");
    if (err == ERR_FILE) printf("\033[0;31mERROR:\033[0m File can't be opened\n\n");
    if (err == ERR_EMPTY_FILE) printf("\033[0;31mERROR:\033[0m File is empty\n\n");
    if (err == ERR_EMPTY_TREE) printf("\033[0;31mERROR:\033[0m Tree is empty\n\n");
}
