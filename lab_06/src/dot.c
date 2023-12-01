#include "dot.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *dot_filename = "./misc/images/word_tree.dot";

static void generate_dot_nodes(FILE *file, word_tree_t *node) {
    if (node == NULL) return;

    fprintf(file, "  \"%s\" [label=\"%s\"];\n", node->word, node->word);

    generate_dot_nodes(file, node->left);
    generate_dot_nodes(file, node->right);
}

static void generate_dot_edges(FILE *file, word_tree_t *node) {
    if (node == NULL) return;

    if (node->left != NULL)
        fprintf(file, "  \"%s\" -> \"%s\";\n", node->word, node->left->word);

    if (node->right != NULL)
        fprintf(file, "  \"%s\" -> \"%s\";\n", node->word, node->right->word);

    generate_dot_edges(file, node->left);
    generate_dot_edges(file, node->right);
}

err_t generate_dot_file(const char *dot_filename, word_tree_t *root) {
    if (root == NULL) return ERR_EMPTY_TREE;

    FILE *file = fopen(dot_filename, "w");
    if (file == NULL) return ERR_FILE;
    fprintf(file, "digraph wordTree {\n");
    fprintf(file, "  node [fontname=\"Arial\"];\n");

    generate_dot_nodes(file, root);
    generate_dot_edges(file, root);

    fprintf(file, "}\n");
    fclose(file);
    return OK;
}

void print_tree_from_dot(const char *dot_filename) {
    char cmd[strlen("xdot ") + strlen(dot_filename) + 1];
    strcpy(cmd, "xdot ");
    strcat(cmd, dot_filename);
    system(cmd);
}
