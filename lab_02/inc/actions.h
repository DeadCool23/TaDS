#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "errs.h"

#include "key.h"

#include "helper.h"

#include "menu.h"
#include "mescmp.h"

#include "sort.h"

#include "tableads.h"
#include "tableio.h"
#include "theatre.h"

#define KEY_TABLE_CHECK(is_used, key_table, table)      \
    do {                                                \
        if (is_used) {                                  \
            key_table.free_key_table(&key_table);       \
            key_table.key_table_gen(&key_table, table); \
            ssort_table_by_keys(&key_table);            \
        }                                               \
    } while (0)

err_t actions(void);

#endif //ACTIONS_H

