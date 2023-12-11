#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("+-----------------------------------------------------------+\n");
    printf("|                         \033[0;34mApp menu\033[0m                          |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1. Load data from file                                    |\n");
    printf("| 2. Add words to data                                      |\n");
    printf("| 3. Find word in tree                                      |\n");
    printf("| 4. Delete words from trees                                |\n");
    printf("| 5. Delete words from hash tables                          |\n");
    printf("| 6. Print all                                              |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| -1. \033[0;31mExit\033[0m                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
}

void traversal_menu(void) {
    printf("\033[0;34mTraversal tree\033[0m \n");
    printf("|----> 1. Preorder traversal\n");
    printf("|----> 2. Inorder traversal\n");
    printf("+----> 3. Postorder traversal\n");
    printf(": ");
}

void data_menu(void) {
    printf("\033[0;34mData\033[0m \n");
    printf("|----> 1. Load generated data\n");
    printf("+----> 2. Load data from file\n");
    printf(": ");
}

void add_menu(void) {
    printf("\033[0;34mWord adder\033[0m\n");
    printf("|----> 1. Add word to simple tree\n");
    printf("|----> 2. Add word to avl tree\n");
    printf("|----> 3. Add word to opened hash table\n");
    printf("|----> 4. Add word to closed hash table\n");
    printf("|----> 5. Add word to hash tables\n");
    printf("|----> 6. Add word to trees\n");
    printf("|----> 7. Add word all data structures\n");
    printf(": ");
}
