/*!
    \file
    File with binary tree functions
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "TreeDump.h"

/*!
    @brief Function that creates binary tree
    \param [out] root - pointer on tree root
    @return The pointer on the tree
*/
Tree* TreeCtor(Node* root) {
    ASSERT(root != NULL, "NULL POINTER WAS PASSED!\n");

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (!tree) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = root;
    tree->size = (TreeNodesCount() - 1) / 2;

    return tree;
}

/*!
    @brief Function that creates node
    \param [in] value - node data
    @return The pointer on the node
*/
Node* CreateNode(NodeData value) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }

    node->data = value;

    return node;
}

/*!
    @brief Function that compare strings in tree
    \param [in]  first_string - first node data
    \param [in] second_string - second node data
    @return The lexicographic difference of the strings
*/
int string_compare(const NodeData first_string, const NodeData secong_string) {
    ASSERT(first_string  != NULL, "NULL PONTER WAS PASSED!\n");
    ASSERT(secong_string != NULL, "NULL PONTER WAS PASSED!\n");

    return strcasecmp(first_string, secong_string);
}

/*!
    @brief Function that creates binary tree
    \param [in]      tree - pointer on tree
    \param [in]      node - pointer on node
    \param [in]     value - node data value
    \param [in] comp_func - pointer on the compare function
    @return The status of the function (return code)
*/
FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    TREE_DUMP(tree, "Start: %s (%s)", __func__, value);

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

    TREE_DUMP(tree, "End: %s (%s)", __func__, value);

    return SUCCESS;
}

/*!
    @brief Function that deletes tree node
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode NodeDtor(Node* node) {
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    FREE(node->data);
    if (node->left) {
        NodeDtor(node->left);
    }

    if (node->right) {
        NodeDtor(node->right);
    }

    FREE(node);

    return SUCCESS;
}

/*!
    @brief Function that deletes binary tree
    \param [out] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode TreeDtor(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    NodeDtor(tree->root);
    FREE(tree);

    return SUCCESS;
}

/*!
    @brief Function that counts nodes from the file
    @return The number of the nodes
*/
size_t TreeNodesCount() {
    static size_t num = 0;
    num++;

    return num;
}

/*!
    @brief Function that skips spaces in the file
    \param  [in] filename - pointer on the file
    \param [out]   symbol - symbol value
    @return The last readed symbol
*/
static int SkipSpacesInFile(FILE* filename, int symbol) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    while (isspace(symbol = fgetc(filename))) {
        ;
    }

    return symbol;
}

/*!
    @brief Function that skips symbols in the file until it meets the necessary one
    \param  [in]    filename - pointer on the file
    \param [out]      symbol - symbol value
    \param  [in] find_symbol - find symbol value
    @return The last readed symbol
*/
static int SkipUntilFindSymbol(FILE* filename, int symbol, char find_symbol) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    while ((symbol = fgetc(filename)) != find_symbol){
        ;
    }

    return symbol;
}

/*!
    @brief Function that reads subtree from the file
    \param [in] filename - pointer on the file
    @return The pointer on the readed node
*/
Node* ReadSubTree(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    int symbol = 0;

    symbol = SkipSpacesInFile(filename, symbol);

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

    symbol      = SkipUntilFindSymbol(filename, symbol, '}');

    return node;
}

/*!
    @brief Function that reads node's data from file
    \param [in] filename - pointer on the file
    @return The pointer on the readed data
*/
char* ReadNodeData(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    char symbol = 0;
    symbol = (char) SkipSpacesInFile(filename, int(symbol));

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

/*!
    @brief Function that writes binary tree in the file
    \param [in] filename - pointer on the file
    \param [in]     tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode WriteTree(FILE* filename, Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n");

    WriteSubTree(filename, tree->root);

    return SUCCESS;
}

/*!
    @brief Function that writes subtree in file
    \param [in] filename - pointer on the file
    \param [in]     node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode WriteSubTree(FILE* filename, Node* node) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

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

/*!
    @brief Function that find element in the binary tree
    \param  [in]       node - pointer on the node
    \param  [in]      value - find node data value
    \param [out]       path - pointer on the node path
    \param [out] logic_path - pointer on the node logic path
    @return The status of the function (return code)
*/
int TreeFindElem(Node* node, const char* value, NodeData* path, int* logic_path) {
    ASSERT(value != NULL, "NULL POINTER WAS PASSED!\n");

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