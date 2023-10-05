#ifndef __ERRS_H__
#define __ERRS_H__

typedef enum {
    OK,
    ERR_IO,
    ERR_MEM,
    ERR_ACT,
    ERR_FILE,
} err_t;

void err_print(err_t err);

#endif //__ERRS_H__

