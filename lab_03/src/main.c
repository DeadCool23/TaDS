#include "main.h"

int main(void) { 
    int act = 0;
    err_t err = OK;
    matrix_t std_matrix = { .data = NULL };
    rvector_t std_vector = { .vector = NULL };
    rvsparce_t sparce_vec = { .els = NULL, .el_j = NULL };
    msparce_t sparce_matrix = { .els = NULL, .el_i = NULL, .el_j = NULL};
    rvector_t std_res = {.vector = NULL }; rvsparce_t sparce_res = { .els = NULL, .el_j = NULL };
    do {
        menu();
        printf("Input choice: ");
        act = 0;
        scanf("%d", &act);
        printf("\033[2J");
        clean_buf();
        switch (act) {
            case 1:
                free_data(&std_matrix, &std_vector, &sparce_matrix, &sparce_vec);
                err = input_data(&std_matrix, &std_vector);
                err = !err ? std_to_sparce_vec(&std_vector, &sparce_vec) : err;
                err = !err ? std_to_sparce_matrix(&std_matrix, &sparce_matrix) : err;
                if (!err) INPUT_PROMT(stdin, "Data loaded \033[0;32msuccessfully\033[0m\n\n");
                break;
            case 2:
                free_data(&std_matrix, &std_vector, &sparce_matrix, &sparce_vec);
                err = generator(&std_matrix, &std_vector);
                if (err == ERR_IO) clean_buf();
                err = !err ? std_to_sparce_vec(&std_vector, &sparce_vec) : err;
                err = !err ? std_to_sparce_matrix(&std_matrix, &sparce_matrix) : err;
                if (!err) INPUT_PROMT(stdin, "Data generated \033[0;32msuccessfully\033[0m\n\n");
                break;
            case 3:
                free_res(&std_res, &sparce_res);
                err = std_mult(&std_vector, &std_matrix, &std_res, false);
                if (!err) OUTPUT_INFO(stdout, "\n\nMultiply vector-column result\n\033[0;34m---\033[0m\n");
                err = !err ? std_to_sparce_vec(&std_res, &sparce_res) : err;
                err = !err ? vector_print(&std_res, &sparce_res) : err;
                break;
            case 4:
                free_res(&std_res, &sparce_res);
                err = sparce_mult(&sparce_vec, &sparce_matrix, &sparce_res, false);
                if (!err) OUTPUT_INFO(stdout, "\n\nMultiply vector-column result\n\033[0;34m---\033[0m\n");
                err = !err ? sparce_to_std_vec(&sparce_res, &std_res) : err;
                err = !err ? vector_print(&std_res, &sparce_res) : err;
                break;
            case 5:
                err = mes_table();
                break;
            case 6:
                if (!std_matrix.data || !std_vector.vector) printf("No \033[4mloaded\033[0m data\n\n");
                else err = output_data(&std_matrix, &sparce_matrix, &std_vector, &sparce_vec);
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            
            default:
                printf("\033[2J");
                err = ERR_ACT;
                break;
        }
        if (err) err_print(err);
        if (err == ERR_MEM) act = -1;
        err = OK;
    } while(act != -1);

    free_data(&std_matrix, &std_vector, &sparce_matrix, &sparce_vec);
    free_res(&std_res, &sparce_res);
    return err;
}
