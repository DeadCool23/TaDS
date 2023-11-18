#include "mes.h"

void print_cmp(const mes_t *list, const mes_t *arr) {
    printf("+----------+-----------+-----------+\n");
    printf("|          |       List|      Array|\n");
    printf("+----------+-----------+-----------+\n");
    printf("| Time, ms | %10zu| %10zu|\n", list->time, arr->time);
    printf("|   Mem, b | %10zu| %10zu|\n", list->mem, arr->mem);
    printf("+----------+-----------+-----------+\n\n");
}
