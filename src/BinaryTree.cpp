#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>

#include "TreeDump.h"

Tree* TreeCtor(Node* root) {
    ASSERT(root != NULL, "NULL POINTER WAS PASSED!\n")

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (!tree) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = root;
    tree->size = 0;

    return tree;
}


Node* CreateNode(NodeData value) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    node->data  = value;

    return node;
}

int string_compare(const NodeData first_string, const NodeData secong_string) {
    ASSERT(first_string != NULL, "NULL PONTER WAS PASSED!\n")
    ASSERT(secong_string != NULL, "NULL PONTER WAS PASSED!\n")

    return strcasecmp(first_string, secong_string);
}

FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    TREE_DUMP(tree, "Start: %s (%s)", __func__, value)

    int comp_res = comp_func(value, node->data);

    if (comp_res <= 0) {
        if (node->left) {
            TreeInsertNode(tree, node->left, value, comp_func);
        } else {
            node->left = CreateNode(value);
        }
    } else {
        if (node->right) {
            TreeInsertNode(tree, node->right, value, comp_func);
        } else {
            node->right = CreateNode(value);
        }
    }

    tree->size += 1;

    TREE_DUMP(tree, "End: %s (%s)", __func__, value)

    return SUCCESS;
}

FuncReturnCode NodeDtor(Node* node) {
    FREE(node->data)
    if (node->left) {
        NodeDtor(node->left);
    }

    if (node->right) {
        NodeDtor(node->right);
    }

    FREE(node)

    return SUCCESS;
}

FuncReturnCode TreeDtor(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    NodeDtor(tree->root);
    FREE(tree)

    return SUCCESS;
}

size_t TreeNodesCount() {
    static size_t num = 0;
    num++;

    return num;
}

Node* ReadSubTree(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    int symbol = 0;

    //* Skip spaces
    while (isspace(symbol = fgetc(filename))) {
        ;
    }

    if (symbol == '*') {
        TreeNodesCount();

        return NULL;
    } else if (symbol != '{') {
        printf(RED("%s: unknown action symbol %c\n"), __func__, symbol);

        return NULL; //abort() ?
    }

    Node* node = CreateNode(ReadNodeData(filename)); // create node

    node->left  = ReadSubTree(filename);
    node->right = ReadSubTree(filename);

    while ((symbol = fgetc(filename)) != '}'){
        ;
    }

    return node;
}

char* ReadNodeData(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    char symbol = 0;
    while (isspace(symbol = fgetc(filename))){
        ;
    }

    if (symbol != '"') {
        printf(RED("ERROR WITH READ TREE FROM FILE1\n"));

        // abort() ?
    }

    char* node_data = (char*) calloc(1, MAX_DATA_SIZE * sizeof(char));
    if (!node_data) {
        printf(RED("MEMORY ERROR!\n"));

        // abort() ?
    }

    fscanf(filename, "%[^\"]", node_data);
    symbol = fgetc(filename);

    return node_data;
}

FuncReturnCode WriteTree(FILE* filename, Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n")

    WriteSubTree(filename, tree->root);

    return SUCCESS;
}

FuncReturnCode WriteSubTree(FILE* filename, Node* node) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    if (node == NULL) {
        fprintf(filename, "* ");

        return SUCCESS;
    }

    fprintf(filename, "{ ");

    fprintf(filename, "\"%s\" ", node->data);
    WriteSubTree(filename, node->left);
    WriteSubTree(filename, node->right);

    fprintf(filename, "} ");

    return SUCCESS;
}