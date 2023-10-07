#include "helper.h"

#include <stdlib.h>
#include <string.h>

void header(char type) {
    size_t TABLE_WIDTH;
    if (type == 't') TABLE_WIDTH = 194;
    else if (type == 'k') TABLE_WIDTH = 38;
    for (size_t i = 0; i < TABLE_WIDTH; i++) printf("\033[0;33m-\033[0m");
    if (type == 't')
        printf("\n\033[0;33m|%-30s|%-30s|%-25s|%-20s|%-30s|%-30s|%-10s|%-10s|\033[0m\n",
        "Theater name", "Perfomance name", "Price USD", 
        "Perfomance type", "Composer name", "Country", 
        "Age", "Duration");
    else if (type == 'k')
        printf("\n\033[0;33m|%5s|%-30s|\033[0m\n",
        "i", "Theatre name");
    for (size_t i = 0; i < TABLE_WIDTH; i++) printf("\033[0;33m-\033[0m");
    printf("\n");
}

void ending(char type) {
    size_t TABLE_WIDTH;
    if (type == 't') TABLE_WIDTH = 194;
    else if (type == 'k') TABLE_WIDTH = 38;
    for (size_t i = 0; i < TABLE_WIDTH; i++) printf("\033[0;33m-\033[0m");
    printf("\n\n");
}

void clean_buf(void) { for (char symb = '0'; symb != '\n' && symb != EOF; symb = getc(stdin)); }

char *get_str(FILE *file, char end, ssize_t size) {
    size_t cnt = 0;
    char symb, *str = NULL;
    while ((symb = getc(file)) != end) {
        if (symb == EOF) goto NUL;
        char *ptmp = realloc(str, (cnt + 1) * sizeof(*str));
        if (!ptmp) return NULL;
        str = ptmp;
        *(str + cnt++) = symb;
        if (size >= 0 && size < (ssize_t)cnt) goto NUL;
    }

    char *ptmp = realloc(str, (cnt + 1) * sizeof(*str));
    if (!ptmp) return NULL;
    str = ptmp;
    *(str + cnt) = '\0';
    if (!strlen(str)) goto NUL;
    return str;

    NUL:
    free(str);
    return NULL;
}

