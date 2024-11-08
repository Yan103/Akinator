#include <stdio.h>
#include <stdlib.h>

#include "Default.h"
#include "BinaryTree.h"
#include "FuncReturnCode.h"
#include "TreeDump.h"

// TODO documentation and checks

Tree* TreeCtor(NodeData root_value) {
    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (!tree) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = CreateNode(root_value, NULL, NULL);
    if (!tree->root) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->size = 1;
    TREE_DUMP(tree, "%s (%d)", __func__, root_value)

    return tree;
}


Node* CreateNode(NodeData value, Node* left, Node* right) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    node->data  = value;
    node->left  =  left;
    node->right = right;

    return node;
}

int NumberCompare(int first_num, int second_num) {
    return first_num - second_num;
}

FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    TREE_DUMP(tree, "Start: %s (%d)", __func__, value)

    int comp_res = comp_func(value, node->data);

    if (comp_res <= 0) {
        if (node->left) {
            TreeInsertNode(tree, node->left, value, comp_func);
        } else {
            node->left = CreateNode(value, NULL, NULL);
        }
    } else {
        if (node->right) {
            TreeInsertNode(tree, node->right, value, comp_func);
        } else {
            node->right = CreateNode(value, NULL, NULL);
        }
    }

    tree->size += 1;

    TREE_DUMP(tree, "End: %s (%d)", __func__, value)

    return SUCCESS;
}

FuncReturnCode NodeDtor(Node* node) {
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