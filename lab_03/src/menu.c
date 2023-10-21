#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("---------------------------------------------------------------------\n");
    printf("|                            \033[0;34mMatrix mult\033[0m                            |\n");
    printf("---------------------------------------------------------------------\n");
    printf("| 1. Input matrix and vector-string                                 |\n");
    printf("| 2. Generate matrix and vector-string                              |\n");
    printf("| 3. Multiply standart vector-string and matrix                     |\n");
    printf("| 4. Multiply sparce vector-string and matrix                       |\n");
    printf("| 5. Comparison efficiency mult of sparce and std matrix            |\n");
    printf("| 6. Print matrix, vector-string                                    |\n");
    printf("---------------------------------------------------------------------\n");
    printf("| -1. \033[0;31mExit\033[0m                                                          |\n");
    printf("---------------------------------------------------------------------\n");
}
