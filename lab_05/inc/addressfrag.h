#ifndef __addressFRAG_H__
#define __addressFRAG_H__

#include "errs.h"
#include <stddef.h>

typedef struct address_frag addresses_t;

addresses_t *adr_init(void);

err_t add_address(addresses_t **adrs, void *address);
void adrs_print(addresses_t *adrs, size_t adr_cnt);

void adr_free(addresses_t *adrs);

#endif // __addressFRAG_H__
