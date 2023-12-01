#include "main.h"

int main(void) {
    int act = 0;
    err_t err = OK;
    char *filename = NULL;
    word_tree_t *tree = tree_init();
    do {
        menu();
        act = 0;
        printf("Input choice: ");
        scanf("%d", &act);
        printf("\033[2J");
        clean_buf();
        switch (act) {
            case 1:
                err = load_act(&tree, &filename);
                if (err == ERR_IO) clean_buf();
                break;
            case 2:
                err = find_act(tree);
                break;
            case 3:
                err = del_act(&tree); 
                break;
            case 4:
                err = traveral_act(tree);
                break;
            case 5:
                err = generate_dot_file(dot_filename, tree);
                if (!err) print_tree_from_dot(dot_filename);
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            case 505:
                cmp_del_from_file_and_tree();
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
