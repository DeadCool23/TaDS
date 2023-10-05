#ifndef __TABLEIO_H__
#define __TABLEIO_H__

#include "theatreio.h"
#include "errs.h"

void table_print(const theatre_table_t *table);
err_t load_table_from_file(FILE *file, theatre_table_t *table);

#endif //__TABLEIO_H__

