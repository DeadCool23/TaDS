#ifndef __HELPER_H__
#define __HELPER_H__

#include <time.h>
#include <stdio.h>

#define RESET_RAND srand(time(NULL))

void clean_buf(void);
ssize_t microseconds_now(void);

#endif //__HELPER_H__
