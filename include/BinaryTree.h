#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Default.h"
#include "BinaryTree.h"
#include "FuncReturnCode.h"

// TODO documentation

const size_t MAX_USER_ANS = 20;

typedef char* NodeData;

struct Node {
    NodeData data;
    Node*    left;
    Node*   right;
};

struct Tree {
    Node*    root;
    size_t   size;
};

const size_t MAX_DATA_SIZE = 500;

typedef int (*CompType) (const NodeData, const NodeData);

Tree* TreeCtor(Node* root);

Node* CreateNode(NodeData value);

int string_compare(const NodeData first_string, const NodeData secong_string);

FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func);

FuncReturnCode TreeDtor(Tree* tree);

FuncReturnCode NodeDtor(Node* node);

Node* ReadSubTree(FILE* filename);

char* ReadNodeData(FILE* filename);

size_t TreeNodesCount();

FuncReturnCode WriteSubTree(FILE* filename, Node* node);

FuncReturnCode WriteTree(FILE* filename, Tree* tree);

int TreeFindElem(Node* node, NodeData value, NodeData* path, int* path1);

#endif // BINARY_TREE_H