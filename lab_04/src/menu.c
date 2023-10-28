#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("+-----------------------------------------------------------+\n");
    printf("|                          \033[0;34mStack\033[0m                            |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1. Push element in stack                                  |\n");
    printf("| 2. Pop element from stack                                 |\n");
    printf("| 3. Check brackets validity                                |\n");
    printf("| 4. Print stack                                            |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| -1. \033[0;31mExit\033[0m                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
}
