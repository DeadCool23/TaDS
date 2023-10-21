#include "vectorio.h"

#include "inout.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static err_t coordinate_vector_read(FILE *f, const rvector_t *vector) {
#define CORDINATES_INPUT_CNT 2
    int els_cnt;
    bool * addi_arr = calloc(vector->size, sizeof(*addi_arr));
    if (!addi_arr) return ERR_MEM;

    err_t err = OK;
    INPUT_PROMT(f, "\nInput \033[4mcount of coordinates\033[0m: ");
    if (fscanf(f, "%d", &els_cnt) != 1)
        ERR_GOTO(err, ERR_IO, END);

    if (els_cnt < 0 || (size_t)els_cnt > vector->size)
        ERR_GOTO(err, ERR_IO, END);

    for (size_t i = 0; i < (size_t)els_cnt; i++) {
        int el_tmp;
        int size_tmp;
        INPUT_PROMT(f, "\n");
        int *inputs[CORDINATES_INPUT_CNT] = { &size_tmp, &el_tmp };
        char *promts[CORDINATES_INPUT_CNT] = { "Input column index: ", "Input value: " };
        for (size_t i = 0; i < CORDINATES_INPUT_CNT; i++) {
            INPUT_PROMT(f, "%s", promts[i]);
            if (fscanf(f, "%d", inputs[i]) != 1)
                ERR_GOTO(err, ERR_IO, END);
        }

        if ((size_tmp <= 0 || (size_t)size_tmp > vector->size) || addi_arr[size_tmp - 1])
            ERR_GOTO(err, ERR_IO, END);

        vector->vector[size_tmp - 1] = el_tmp;
        addi_arr[size_tmp - 1] = true;
    }
    
    END:
    free(addi_arr);
    return !err ? (fileno(f) != STDIN_FILENO) ? (is_eof(f) ? OK : ERR_FILE) : OK : err;
#undef CORDINATES_INPUT_CNT
}

static err_t std_vector_read(FILE *f, rvector_t *vector) {
    err_t err = OK;
    INPUT_PROMT(f, "Input vector:\n");
    for (size_t i = 0; i < vector->size && !err; i++)
            if (scanf("%d", &vector->vector[i]) != 1)
                err = ERR_IO;

    return !err ? (fileno(f) != STDIN_FILENO) ? (is_eof(f) ? OK : ERR_FILE) : OK : err;
}

static err_t vector_create(FILE *f, rvector_t *vector, input_t type) {
    int tmp_size;
    if (fileno(f) != STDIN_FILENO) {
        if (fscanf(f, "%d", &tmp_size) != 1) return ERR_IO;
        if (tmp_size <= 0) return ERR_IO;
        vector->size = tmp_size;
    }
    
    vector->vector = calloc(vector->size, sizeof(*vector->vector));
    if (!vector->vector) return ERR_MEM;
    
    err_t err = OK;
    switch (type) {
        case COORDINATES:
            err = coordinate_vector_read(f, vector);
            break;
        case STD_INPUT:
            err = std_vector_read(f, vector);
            break;
        default:
            err = ERR_CMD;
            break;
    }

    if (err) free(vector->vector);
    if (err) vector->vector = NULL;
    return err;
}

err_t vector_input(rvector_t *vector) {
    char tmp;
    input_t type;
    INPUT_PROMT(stdin, "Choose type of vector input\n[S:Standart, C:Coordinates]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();
    switch (tmp) {
        case 'S':
            type = STD_INPUT;
            break;
        case 'C':
            type = COORDINATES;
            break;
        default:
            return ERR_CMD;
    }

    INPUT_PROMT(stdin, "\nEntire vector from\n[S:Stdin, F:File]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();

    err_t err = OK;
    char *filename = NULL;
    switch (tmp) {
        case 'S':
            INPUT_PROMT(stdin, "\nVector size: %zu\n", vector->size);
            err = vector_create(stdin, vector, type);
            break;
        case 'F':
            INPUT_PROMT(stdin, "Input filename: ");
            if (!(filename = get_str(stdin, '\n', -1))) err = ERR_IO;
            if (!err) {
                FILE *file = fopen(filename, "r");
                if (file == NULL) err = ERR_FILE;
                if (!err) {
                    err = vector_create(file, vector, type);
                    fclose(file);
                }
            }
            free(filename);
            break;
        default:
            err = ERR_CMD;
            break;
    }
    return err;
}

static void std_vector_print(FILE *file, const rvector_t *vector) {
    OUTPUT_INFO(file, "\nVector size: ");
    fprintf(file, "%zu\n", vector->size);
    
    OUTPUT_INFO(file, "Vector: ");
    for (size_t i = 0; i < vector->size; i++)
        fprintf(file, "%-4d ", vector->vector[i]);
    fprintf(file, "\n\n");
}

static void sparce_vector_print(FILE *file, const rvsparce_t *vector) {
    OUTPUT_INFO(file, "\nVector size: "); 
    fprintf(file, "%zu\n", vector->size);

    OUTPUT_INFO(file, "\nEls: ");
    for (size_t i = 0; i < vector->els_cnt; i++)
        fprintf(file, "%-4d ", vector->els[i]);
    OUTPUT_INFO(file, "\nJEls: ");
    for (size_t i = 0; i < vector->els_cnt; i++)
        fprintf(file, "%-4zu ", vector->el_j[i]);
    fprintf(file, "\n\n");
}

err_t vector_print(const rvector_t *vector, const rvsparce_t *vsparce) {
    char tmp;
    INPUT_PROMT(stdin, "Print \033[1mvector\033[0m\n[C:Classical, S:Sparce]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();
    switch (tmp) {
        case 'C':
            std_vector_print(stdout, vector);
            break;
        case 'S':
            sparce_vector_print(stdout, vsparce);
            break;
        default:
            return ERR_CMD;
    }
    return OK;
}
