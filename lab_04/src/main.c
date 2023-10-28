#include "main.h"
#include "brackets.h"

int main(void) { 
    int act = 0;
    err_t err = OK;
    astack_t arr_s; as_init(&arr_s);
    lstack_t list_s; ls_init(&list_s);
    do {
        menu();
        printf("Input choice: ");
        act = 0;
        scanf("%d", &act);
        printf("\033[2J");
        clean_buf();
        switch (act) {
            case 1:
                err = pusher(&arr_s, &list_s);
                if (err == ERR_IO) clean_buf();
                break;
            case 2:
                err = poper(&arr_s, &list_s);
                if (err == ERR_IO) clean_buf();
                break;
            case 3:
                err = brackets_validate();
                break;
            case 4:
                err = printer(&arr_s, &list_s);
                break;
            case 101:
                get_mes();
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            
            default:
                err = ERR_ACT;
                break;
        }
        if (err) err_print(err);
        err = OK;
    } while(act != -1);

    return err;
}
