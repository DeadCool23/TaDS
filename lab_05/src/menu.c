#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("+-----------------------------------------------------------+\n");
    printf("|                          \033[0;34mQueue\033[0m                            |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1. Requests handler on array                              |\n");
    printf("| 2. Requests handler on list                               |\n");
    printf("| 3. Cmp of handler on array and list                       |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| -1. \033[0;31mExit\033[0m                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
}
