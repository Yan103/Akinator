#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>

#include "FuncReturnCode.h"

// TODO documentation

typedef int NodeData;

struct Node {
    NodeData data;
    Node*    left;
    Node*   right;
};

struct Tree {
    Node*    root;
    size_t   size;
};

typedef NodeData (*CompType) (NodeData, NodeData);

Tree* TreeCtor(NodeData root_value);

Node* CreateNode(NodeData value, Node* left, Node* right);

int NumberCompare(int first_num, int second_num);

FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func);

FuncReturnCode TreeDtor(Tree* tree);

FuncReturnCode NodeDtor(Node* node);

#endif // BINARY_TREE_H