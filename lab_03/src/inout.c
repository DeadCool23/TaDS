#include "inout.h"

#include "helper.h"

err_t input_data(matrix_t *matrix, rvector_t *vector) {
    err_t err = OK;
    INPUT_PROMT(stdin, "\033[0;34mVector-string and matrix input\033[0m\n------------------------------\n");
    INPUT_PROMT(stdin, "\033[1mMatrix\033[0m input\n\n");
    err = matrix_input(matrix);
    if (!err) vector->size = matrix->rows;
    if (!err) INPUT_PROMT(stdin, "\n\033[0;34m---\033[0m\n\033[1mVector-column\033[0m input\n\n");
    if (!err || err == ERR_IO) clean_buf();
    err = !err ? vector_input(vector) : err;
    INPUT_PROMT(stdin, "\n");
    return err;
}

err_t output_data(const matrix_t *matrix, const msparce_t *smatrix, 
                  const rvector_t *vector, const rvsparce_t *svector) {
    char tmp;
    err_t err = OK;
    INPUT_PROMT(stdin, "Output [V:Vector, M:Matrix, A:All]: ");
    if (fscanf(stdin, "%c", &tmp) != 1) return ERR_IO;
    clean_buf();
    switch (tmp) {
        case 'V':
            INPUT_PROMT(stdin, "\n\033[1mVector\033[0m output\n---\n\n");
            return vector_print(vector, svector);
        case 'M':
            INPUT_PROMT(stdin, "\n\033[1mMatrix\033[0m output\n---\n\n");
            return matrix_print(matrix, smatrix);
        case 'A':
            INPUT_PROMT(stdin, "\n\033[1mMatrix\033[0m output\n---\n\n");
            err = matrix_print(matrix, smatrix);
            if (!err) {
                INPUT_PROMT(stdin, "\n\033[1mVector\033[0m output\n---\n\n");
                err = vector_print(vector, svector);
            }
            return err;
        default:
            return ERR_CMD;
    }
    if (!err) INPUT_PROMT(stdin, "\n");
    return err;
}