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
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = root;
    tree->size = (TreeNodesCount() - 1) / 2;

    return tree;
}


Node* CreateNode(NodeData value) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

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
// todo assert
FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    TREE_DUMP(tree, 0, "Start: %s (%s)", __func__, value)

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

    TREE_DUMP(tree, 0, "End: %s (%s)", __func__, value)

    return SUCCESS;
}
// todo check if null
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
    // todo to function
    while (isspace(symbol = fgetc(filename))) {
        ;
    }

    if (symbol == '*') {
        TreeNodesCount();

        return NULL;
    } else if (symbol != '{') {
        fprintf(stderr, RED("%s: unknown action symbol %c\n"), __func__, symbol);

        return NULL; //abort() ?
    }

    Node* node  = CreateNode(ReadNodeData(filename)); // create node
    //int child_count = 0;
    // todo return n children resursively
    node->left  = ReadSubTree(filename);
    node->right = ReadSubTree(filename);

    // todo to func
    while ((symbol = fgetc(filename)) != '}'){
        ;
    }

    return node;
}

char* ReadNodeData(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    // todo to func
    char symbol = 0;
    while (isspace(symbol = (char)fgetc(filename))){
        ;
    }

    if (symbol != '"') {
        fprintf(stderr, RED("ERROR WITH READ TREE FROM FILE1\n"));

        // abort() ?
    }

    char* node_data = (char*) calloc(1, MAX_DATA_SIZE * sizeof(char));
    if (!node_data) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        // abort() ?
    }

    fscanf(filename, "%[^\"]", node_data);
    symbol = (char)fgetc(filename);

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

int TreeFindElem(Node* node, const char* value, NodeData* path, int* logic_path) {
    ASSERT(value != NULL, "NULL POINTER WAS PASSED!\n")

    if (node == NULL) {
        return 0;
    }

    *path = node->data; path++;
    if (strcasecmp(node->data, value) == 0) {
        return 1;
    }

    *logic_path = -1; logic_path++;
    if (node->left) {
        if (TreeFindElem(node->left, value, path, logic_path)) {

            return 1;
        }
    }
    logic_path--;

    *logic_path = 1; logic_path++;
    if (node->right) {
        if (TreeFindElem(node->right, value, path, logic_path)) {
            return 1;
        }
    }
    logic_path--;

    path--;
    return 0;
}