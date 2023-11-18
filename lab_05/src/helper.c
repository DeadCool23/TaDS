#include "helper.h"

#include <stdio.h>
#include <sys/time.h>

void clean_buf(void) { for (char symb = getc(stdin); symb != '\n' && symb != EOF; symb = getc(stdin)); }

ssize_t microseconds_now(void) {
    struct timeval val;

    if (gettimeofday(&val, 0))
        return (ssize_t) -1;

    return val.tv_sec * 1000ULL * 1000ULL + val.tv_usec;
}
