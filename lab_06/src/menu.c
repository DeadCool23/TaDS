#include "menu.h"

#include <stdio.h>

void menu(void) {
    printf("+-----------------------------------------------------------+\n");
    printf("|                    \033[0;34mBinary search tree\033[0m                     |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1. Load tree from file                                    |\n");
    printf("| 2. Find word in tree                                      |\n");
    printf("| 3. Delete words from tree                                 |\n");
    printf("| 4. Tree traversal                                         |\n");
    printf("| 5. Print tree                                             |\n");
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
    printf("\033[0;34mTree data\033[0m \n");
    printf("|----> 1. Load generated data\n");
    printf("+----> 2. Load data from file\n");
    printf(": ");
}
