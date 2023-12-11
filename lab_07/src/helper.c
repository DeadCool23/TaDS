#include "helper.h"

#include <time.h>
#include <stdio.h>
#include <sys/time.h>

void clean_buf(void) { for (char symb = getc(stdin); symb != '\n' && symb != EOF; symb = getc(stdin)); }

bool is_eof(FILE *file) {
    char end = getc(file);
    if (end == EOF)
        return true;
    if (end == '\n') {
        char endd = getc(file);
        if (endd == EOF)
            return true;
        ungetc(endd, file);
    }
    ungetc(end, file);
    return false;
}

ssize_t microseconds_now(void) {
    struct timeval val;

    if (gettimeofday(&val, 0))
        return (ssize_t) -1;

    return val.tv_sec * 1000ULL * 1000ULL + val.tv_usec;
}
