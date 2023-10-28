#ifndef __HELPER_H__
#define __HELPER_H__

#ifdef _WIN64
#include <stdio.h>
#include <unistd.h>

char *get_str(FILE *file, char end, ssize_t size);
#endif // _WIN64

void clean_buf(void);

#endif //__HELPER_H__
