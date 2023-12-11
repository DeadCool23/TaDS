#include "main.h"

int main(void) {
    int act = 0;
    err_t err = OK;
    char *filename = NULL;
    word_tree_t *tree = tree_init();
    word_tree_t *balanced_tree = tree_init();
    chash_table_t ctable; chash_init(&ctable, BEG_SIZE);
    ohash_table_t otable; ohash_init(&otable, BEG_SIZE);
    do {
        menu();
        act = 0;
        printf("Input choice: ");
        scanf("%d", &act);
        printf("\033[2J");
        clean_buf();
        switch (act) {
            case 1:
                err = load_act(&tree, &balanced_tree, &otable, &ctable, &filename);
                if (err == ERR_IO) clean_buf();
                break;
            case 2:
                err = add_act(&tree, &balanced_tree, &otable, &ctable);
                break;
            case 3:
                err = find_act(tree);
                break;
            case 4:
                err = del_tree_act(&tree, &balanced_tree); 
                break;
            case 5:
                err = del_hash_act(&ctable, &otable);
                break;
            case 6:
                err = generate_dot_file(dot_filename, tree, "word_tree");
                if (!err) print_tree_from_dot(dot_filename);
                err = generate_dot_file(balanced_dot_filename, balanced_tree, "balanced_word_tree");
                if (!err) print_tree_from_dot(balanced_dot_filename);
                chash_print(&ctable);
                printf("\n\n");
                ohash_print(&otable);
                err = OK;
                break;
            case -1:
                printf("Exit the application...\n\n");
                break;
            case 505:
                cmp_del_from_file_and_trees_and_hash_tables();
                break;
            default:
                err = ERR_ACT;
                break;
        }
        if (err) err_print(err);
        err = OK;
    } while(act != -1);

    tree_destroy(balanced_tree);
    tree_destroy(tree);
    ohash_free(&otable);
    chash_free(&ctable);
    free(filename);
    return err;
}
