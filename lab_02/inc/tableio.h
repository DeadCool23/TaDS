#ifndef __TABLEIO_H__
#define __TABLEIO_H__

#include "errs.h"
#include "theatreio.h"

void table_print(const theatre_table_t *table);
err_t load_table_from_file(FILE *file, theatre_table_t *table);
void load_table_to_formated_file(FILE *file, const theatre_table_t *table);

#endif //__TABLEIO_H__

