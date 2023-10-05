#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("------------------------|\033[1;32mTheaters repertoire\033[0m|------------------------\n");
    printf("| 1. Load data from file                                            |\n");
    printf("| 2. Add data to table                                              |\n");
    printf("| 3. Remove data by thetre name                                     |\n");
    printf("| 4. Print data                                                     |\n");
    printf("| 5. Print sorted by theatre name table                             |\n");
    printf("| 6. Print sorted by theatre name table using key table             |\n");
    printf("| 7. Search ballets by age and duration                             |\n");
    printf("| 8. Print cmp of table or key table sorting                        |\n");
    printf("| 9. Print cmp of faster and slower sorting algorithms              |\n");
    printf("|10. Clean table                                                    |\n");
    printf("---------------------------------------------------------------------\n");
    printf("| -1. \033[0;31mExit\033[0m                                                          |\n");
    printf("---------------------------------------------------------------------\n");
}

