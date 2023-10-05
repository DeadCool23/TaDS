#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <unistd.h>

void header(char type);
void ending(char type);

void clean_buf(void);

char *get_str(FILE *file, char end, ssize_t size);

#endif //__HELPER_H__

