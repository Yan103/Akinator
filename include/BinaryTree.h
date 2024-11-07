#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>

#include "FuncReturnCode.h"

typedef int NodeData;

struct Node {
    NodeData data;
    Node*    left;
    Node*   right;
};

struct Tree {
    Node*  root;
    size_t size;
};

Node* CreateNode(NodeData value);

Tree* TreeCtor();
FuncReturnCode TreeDtor(Tree* tree);

#endif // BINARY_TREE_H