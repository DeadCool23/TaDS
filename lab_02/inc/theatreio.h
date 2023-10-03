#ifndef __THEATREIO_H__
#define __THEATREIO_H__

#include <stdio.h>
#include <string.h>

#include "errs.h"
#include "theatre.h"

#define EMPTY_FIELD "-"

#define FGETS_CHECKER(buf, len, file)                                               \
    do {                                                                            \
        if (!fgets(buf, len + 1, file)) return ERR_IO;                              \
        char *buf_end = strchr(buf, '\n');                                          \
        if (buf_end == NULL) return ERR_IO;                                         \
        else *buf_end = '\0';                                                       \
        if (!strlen(buf)) return ERR_IO;                                            \
    } while (0)

#define SCANF_CHECKER(n, ...)                                                        \
    do {                                                                             \
        if (scanf(__VA_ARGS__) != n)                                                 \
            return ERR_IO;                                                           \
    } while (0)

void theatre_print(const theatre_t *theatre);
err_t load_theatre_from_file(FILE *file, theatre_t *theatre);

#endif //__THEATREIO_H__