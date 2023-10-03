#ifndef __TABLE_ADD_DEL_SEARCH_H__
#define __TABLE_ADD_DEL_SEARCH_H__

#include "errs.h"

#include "helper.h"

#include "theatre.h"
#include "theatreio.h"

err_t add_to_table(theatre_table_t *table);
err_t del_theatres_by_name(theatre_table_t *table);
err_t search_ballet_by_fiealds(theatre_table_t *table);

void free_table(theatre_table_t *table);

#endif //__TABLE_ADD_DEL_SEARCH_H__