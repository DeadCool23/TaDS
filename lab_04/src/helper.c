#include "helper.h"

#include <stdio.h>

void clean_buf(void) { for (char symb = getc(stdin); symb != '\n' && symb != EOF; symb = getc(stdin)); }

#ifdef _WIN64
#include <stdlib.h>
#include <string.h>

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
#endif