#include  "actions.h"

err_t actions(void) {
    int act = 0;
    err_t err = OK;
    bool is_key = false;
    key_table_t key_table = {.keys = NULL};
    theatre_table_t table = {.size = 0, .theatres = NULL};
    mes_t sort_mes[SORT_TYPE][ARR_TYPE];
    init(&key_table);
    do {
        menu();
        printf("Input choice: ");

        act = 0;
        scanf("%d", &act);
        printf("\033[2J"); // Очистка экрана
        clean_buf();
        switch (act) {
            case 1:
                printf("Input \033[4mfilename\033[0m: ");
                char *filename = get_str(stdin, '\n', -1);
                FILE *file = fopen(filename, "r");
                if (file == NULL) err = ERR_FILE;
                if (!err) {
                    err = load_table_from_file(file, &table);
                    if (!err && table.size) printf("Data loaded \033[0;32msuccessfully\033[0m from file \033[4m%s\033[0m\n\n", filename);
                    else if (!err && !table.size) printf("\033[0;31mNo data\033[0m in %s\n\n", filename);
                    is_key = false;
                    fclose(file);
                }
                break;
            case 2:
                err = add_to_table(&table);
                if (!err) printf("Data added \033[0;32msuccessfully\033[0m\n\n");
                KEY_TABLE_CHECK(is_key, key_table, table);
                break;
            case 3:
                err = del_theatres_by_name(&table);
                if (!err) printf("Data removed \033[0;32msuccessfully\033[0m\n\n");
                KEY_TABLE_CHECK(is_key, key_table, table);
                break;
            case 4:
                table_print(&table);
                break;
            case 5:
                if(table.size) ssort_table(&table);
                printf("Data sorted \033[0;32msuccessfully\033[0m\n\n");
                table_print(&table);
                break;
            case 6:
                if (!is_key) {
                    key_table.key_table_gen(&key_table, table);
                    if(table.size) ssort_table_by_keys(&key_table);
                    if (!err) printf("Data sorted by keys \033[0;32msuccessfully\033[0m\n\n");
                    is_key = true;
                }
                printf("What \033[4mtable\033[0m print? \033[3m[K:Key, T,Table]\033[0m ");
                char table_printer;
                SCANF_CHECKER(1, "%c", &table_printer);
                clean_buf();
                if (table_printer == 'T') key_table_print(&key_table, &table);
                else if (table_printer == 'K') keys_print(&key_table);
                else printf("\033[0;31mERROR:\033[0m Incorrect key\n\n");
                break;
            case 7:
                err = search_ballet_by_fiealds(&table);
                break;
            case 8:
                if (!is_key) key_table.key_table_gen(&key_table, table);
                else {
                    key_table.free_key_table(&key_table);
                    key_table.key_table_gen(&key_table, table);
                }
                get_measures(sort_mes, &key_table, &table);
                print_arr_cmp_table(sort_mes);
                !is_key ? key_table.free_key_table(&key_table) : ssort_table_by_keys(&key_table);
                break;
            case 9:
                if (!is_key) key_table.key_table_gen(&key_table, table);
                else {
                    key_table.free_key_table(&key_table);
                    key_table.key_table_gen(&key_table, table);
                }
                get_measures(sort_mes, &key_table, &table);
                print_sort_cmp_table(sort_mes);
                !is_key ? key_table.free_key_table(&key_table) : ssort_table_by_keys(&key_table);
                break;
            case 10:
                printf("Input \033[4mfilename\033[0m: ");
                filename = get_str(stdin, '\n', -1);
                file = fopen(filename, "w");
                if (file == NULL) err = ERR_FILE;
                if (!err) {
                    if (!table.size) printf("\033[4mNo data in table\033[0m\n\n");
                    else {
                        load_table_to_formated_file(file, &table);
                        printf("Data \033[0;32msuccessfully\033[0m loaded in %s\n\n", filename);
                    }
                    fclose(file);
                }
                break;
            case 11:
                free_table(&table);
                key_table.free_key_table(&key_table);
                printf("Data \033[0;32msuccessfully\033[0m cleaned\n\n");
                is_key = false;
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            
            default:
                err = ERR_ACT;
                break;
        }
        if (err) err_print(err);
        if (err == ERR_MEM) act = -1;
        err = OK;
    } while(act != -1);

    key_table.free_key_table(&key_table);
    free(table.theatres);
    return err;
}

