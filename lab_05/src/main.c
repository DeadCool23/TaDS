#include "main.h"
#include <stdlib.h>

int main(void) { 
    int act = 0;
    err_t err = OK;
    mes_t list_mes, arr_mes;
    do {
        menu();
        printf("Input choice: ");
        act = 0;
        scanf("%d", &act);
        printf("\033[2J");
        clean_buf();
        switch (act) {
            case 1:
                err = arr_service_call(false);
                if (err == ERR_IO) clean_buf();
                break;
            case 2:
                err = list_service_call(false);
                if (err == ERR_IO) clean_buf();
                break;
            case 3:
                err = arr_service_call(true, &arr_mes);
                err = !err ? list_service_call(true, &list_mes) : err;
                if (!err) print_cmp(&list_mes, &arr_mes);
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            
            default:
                err = ERR_ACT;
                break;
        }
        if (err) err_print(err);
        if (err == ERR_MEM || err == ERR_CHANCE)
            act = -1;
        else
            err = OK;
    } while(act != -1);
    return err;
}
