#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void clean_buf(void);
bool is_eof(FILE *file);

#endif //__HELPER_H__
