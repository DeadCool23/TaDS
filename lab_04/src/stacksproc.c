#include "stacksproc.h"

#include <stdio.h>

#define TABLE_SIZE 20
#define ADRESSES_TABLE_SIZE 15

#define PARSE_LINE(size)                                \
    do {                                                \
        printf("+");                                    \
        for (size_t i = 1; i < size; i++) printf("-");  \
        printf("+\n");                                  \
    } while (0)

#define HEADER                                          \
    do {                                                \
        PARSE_LINE(TABLE_SIZE);                         \
        printf("|elem|%14s|\n", "adress");              \
        PARSE_LINE(TABLE_SIZE);                         \
    } while (0)


#define FREE_ADRESSES_PRINT                                   \
    do {                                                      \
        char c, end;                                          \
        printf("Print free adresses ?\n[Y: Yes; N: No]: ");   \
        SCANF_CHECKER(2, "%c%c", &c, &end);                   \
        if (end != '\n') return ERR_IO;                       \
        switch (c) {                                          \
            case 'Y':                                         \
                if (!free_adresses.size) {                    \
                    printf("No free adresses\n");             \
                    break;                                    \
                }                                             \
                printf("\n\n");                               \
                PARSE_LINE(ADRESSES_TABLE_SIZE);              \
                printf("|\033[34m%-14s\033[0m|\n", "Adress"); \
                PARSE_LINE(ADRESSES_TABLE_SIZE);              \
                fadress_print();                              \
                PARSE_LINE(ADRESSES_TABLE_SIZE);              \
                break;                                        \
            case 'N':                                         \
                break;                                        \
            default:                                          \
                return ERR_CMD;                               \
        }                                                     \
    } while (0)


err_t poper(astack_t *as, lstack_t *ls) {
    printf("Pop element from [1: Array stack; 2: List stack; 0: All]\n: ");
    char choice, end;
    char stacks_pop[STACK_SIZE];
    SCANF_CHECKER(2, "%c%c", &choice, &end);
    if (end != '\n') return ERR_IO;
    switch (choice) {
        case '1':
            stacks_pop[ARR_STACK] = as_pop(as);
            stacks_pop[ARR_STACK] 
            ? printf("\n\033[0;34mARRAY STACK:\033[0m Poped element: %c\n\n", stacks_pop[ARR_STACK])
            : printf("\n\033[0;31mARRAY STACK:\033[0m Stack is \033[31mempty\033[0m\n\n" );
            break;
        case '2':
            stacks_pop[LIST_STACK] = ls_pop(ls);
            stacks_pop[LIST_STACK] 
            ? printf("\n\033[0;34mLIST STACK:\033[0m Poped element: %c\n\n", stacks_pop[LIST_STACK])
            : printf("\n\033[0;34mLIST STACK:\033[0m Stack is \033[31mempty\033[0m\n\n" );
            break;
        case '0':
            stacks_pop[ARR_STACK] = as_pop(as); stacks_pop[LIST_STACK] = ls_pop(ls);
            stacks_pop[ARR_STACK] 
            ? printf("\n\033[0;34mARRAY STACK:\033[0m Poped element: %c\n", stacks_pop[ARR_STACK])
            : printf("\n\033[0;34mARRAY STACK:\033[0m Stack is \033[31mempty\033[0m\n");
            stacks_pop[LIST_STACK] 
            ? printf("\033[0;34mLIST STACK:\033[0m Poped element: %c\n\n", stacks_pop[LIST_STACK])
            : printf("\033[0;34mLIST STACK:\033[0m Stack is \033[31mempty\033[0m\n\n");
            break;
        default:
            return ERR_CMD;
    }
    return OK;
}

err_t pusher(astack_t *as, lstack_t *ls) {
    char el, choice, end;
    printf("Input pushing element: ");
    SCANF_CHECKER(2, "%c%c", &el, &end);
    if (end != '\n') return ERR_IO;
    printf("Push element to [1: Array stack; 2: List stack; 0: All]\n: ");
    err_t stacks_push[STACK_SIZE];
    SCANF_CHECKER(2, "%c%c", &choice, &end);
    if (end != '\n') return ERR_IO;
    switch (choice) {
        case '1':
            stacks_push[ARR_STACK] = as_push(as, el);
            !stacks_push[ARR_STACK] 
            ? printf( "\n\033[0;34mARRAY STACK:\033[0m Element pushed \033[0;32msuccessfully\033[0m\n\n")
            : printf( "\n\033[0;34mARRAY STACK:\033[0m Stack \033[31moverflow\033[0m\n\n" );
            break;
        case '2':
            stacks_push[LIST_STACK] = ls_push(ls, el);
            printf( !stacks_push[LIST_STACK] 
            ? "\n\033[0;34mARRAY STACK:\033[0m Element pushed \033[0;32msuccessfully\033[0m\n\n"
            : "\n\033[0;34mARRAY STACK:\033[0m Stack \033[31moverflow\033[0m\n\n" );
            break;
        case '0':
            stacks_push[ARR_STACK] = as_push(as, el); stacks_push[LIST_STACK] = ls_push(ls, el);
            !stacks_push[ARR_STACK] 
            ? printf("\n\033[0;34mARRAY STACK:\033[0m Element pushed \033[0;32msuccessfully\033[0m\n")
            : printf("\n\033[0;34mARRAY STACK:\033[0m Stack \033[31moverflow\033[0m\n" );
            !stacks_push[LIST_STACK] 
            ? printf("\033[0;34mARRAY STACK:\033[0m Element pushed \033[0;32msuccessfully\033[0m\n\n")
            : printf("\033[0;34mARRAY STACK:\033[0m Stack \033[31moverflow\033[0m\n\n" );
            break;
        default:
            return ERR_CMD;
    }
    return OK;
}

err_t printer(astack_t *as, lstack_t *ls) {
    printf("Print [1: Array stack; 2: List stack; 0: All]\n: ");
    char choice, end;
    SCANF_CHECKER(2, "%c%c", &choice, &end);
    if (end != '\n') return ERR_IO;
    switch (choice) {
        case '1':
            if (as_empty(as)) {
                printf("No elements in stack");
                break;
            }
            HEADER;
            as_print(as);
            PARSE_LINE(TABLE_SIZE);
            break;
        case '2':
            if (ls_empty(ls)) {
                printf("No elements in stack");
                break;
            }
            HEADER;
            ls_print(ls);
            PARSE_LINE(TABLE_SIZE);
            FREE_ADRESSES_PRINT;
            break;
        case '0':
            PARSE_LINE(TABLE_SIZE);
            printf("|\033[34m%19s\033[0m|\n", "Array stack");
            if (as_empty(as)) {
                printf("|%19s|\n", "No els in stack");
            } else {
                HEADER;
                as_print(as);
            }
            PARSE_LINE(TABLE_SIZE);
            printf("\n\n");
            PARSE_LINE(TABLE_SIZE);
            printf("|\033[34m%19s\033[0m|\n", "List stack");
            if (ls_empty(ls)) {
                printf("|%19s|\n", "No els in stack");
            } else {
                HEADER;
                ls_print(ls);
            }
            PARSE_LINE(TABLE_SIZE);
            printf("\n\n");
            FREE_ADRESSES_PRINT;
            break;
        default:
            return ERR_CMD;
    }
    printf("\n\n");
    return OK;
}
