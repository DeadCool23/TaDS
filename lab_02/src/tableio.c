#include "tableio.h"
#include "helper.h"

#include <stdlib.h>

void table_print(const theatre_table_t *table) {
    if (!table->size) {
        printf("\033[4mNo loaded data\033[0m\n\n");
    } else {
        header('t');
        for (size_t i = 0; i < table->size; i++)
            theatre_print(&table->theatres[i]);
        ending('t');
    }
}

err_t load_table_from_file(FILE *file, theatre_table_t *table) {
    char end;
    theatre_t theatre_i;
    for (; (end = getc(file)) != EOF; table->size++) {
        ungetc(end, file);
        err_t err = load_theatre_from_file(file, &theatre_i);
        if (err != OK) {
            free(table->theatres);
            table->theatres = NULL;
            table->size = 0;
            return err;
        }
        theatre_t *ptmp = realloc(table->theatres, (table->size + 1) * sizeof(*table->theatres));
        table->theatres = ptmp;
        if (!ptmp) {
            table->size = 0;
            return ERR_MEM;
        }
        table->theatres[table->size] = theatre_i;
        end = getc(file);
        if (end != '\n' && end != EOF) {
            free(table->theatres);
            table->theatres = NULL;
            table->size = 0;
            return ERR_FILE;
        }
        if (end == EOF) ungetc(end, file);
    }
    return OK;
}

void load_table_to_formated_file(FILE *file, const theatre_table_t *table) {
    for (size_t i = 0; i < table->size; i++)
        load_theatre_to_formated_file(file, &table->theatres[i]);
}