/*!
    \file
    File with binary tree functions
*/

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Default.h"
#include "BinaryTree.h"
#include "FuncReturnCode.h"

/// @brief Constant for user's answer buffer
const size_t MAX_USER_ANS = 20;

/// @brief Constant for node data buffer
const size_t MAX_DATA_SIZE = 500;

/// @brief Type of items in a nodes' data
typedef char* NodeData;

/// @brief Structure tree node
struct Node {
    NodeData data;
    Node*    left;
    Node*   right;
};

/// @brief Structure binary tree
struct Tree {
    Node*    root;
    size_t   size;
};

/// @brief Structure node path
struct NodePath {
    char**      path;
    NodeData    data;
    int*  logic_path;
};

/// @brief Special typedef for my comparate functions
typedef int (*CompType) (const NodeData, const NodeData);

/*!
    @brief Function that creates binary tree
    \param [out] root - pointer on tree root
    @return The pointer on the tree
*/
Tree* TreeCtor(Node* root);

/*!
    @brief Function that creates node
    \param [in] value - node data
    @return The pointer on the node
*/
Node* CreateNode(NodeData value);

/*!
    @brief Function that compare strings in tree
    \param [in]  first_string - first node data
    \param [in] second_string - second node data
    @return The lexicographic difference of the strings
*/
int string_compare(const NodeData first_string, const NodeData secong_string);

/*!
    @brief Function that creates binary tree
    \param [in]      tree - pointer on tree
    \param [in]      node - pointer on node
    \param [in]     value - node data value
    \param [in] comp_func - pointer on the compare function
    @return The status of the function (return code)
*/
FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func);

/*!
    @brief Function that deletes binary tree
    \param [out] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode TreeDtor(Tree* tree);

/*!
    @brief Function that deletes tree node
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode NodeDtor(Node* node);

/*!
    @brief Function that reads subtree from the file
    \param [in] filename - pointer on the file
    @return The pointer on the readed node
*/
Node* ReadSubTree(FILE* filename);

/*!
    @brief Function that reads node's data from file
    \param [in] filename - pointer on the file
    @return The pointer on the readed data
*/
char* ReadNodeData(FILE* filename);

/*!
    @brief Function that counts nodes from the file
    @return The number of the nodes
*/
size_t TreeNodesCount();

/*!
    @brief Function that writes subtree in file
    \param [in] filename - pointer on the file
    \param [in]     node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode WriteSubTree(FILE* filename, Node* node);

/*!
    @brief Function that writes binary tree in the file
    \param [in] filename - pointer on the file
    \param [in]     tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode WriteTree(FILE* filename, Tree* tree);

/*!
    @brief Function that find element in the binary tree
    \param  [in]       node - pointer on the node
    \param  [in]      value - find node data value
    \param [out]       path - pointer on the node path
    \param [out] logic_path - pointer on the node logic path
    @return The status of the function (return code)
*/
int TreeFindElem(Node* node, const char* value, NodeData* path, int* logic_path);

#endif // BINARY_TREE_H