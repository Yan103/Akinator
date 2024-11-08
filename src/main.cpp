#include <stdio.h>

#include "FuncReturnCode.h"
#include "BinaryTree.h"
#include "Default.h"

int main() {
    Tree* tree = TreeCtor(50);

    TreeInsertNode(tree, tree->root, 70, NumberCompare);
    TreeInsertNode(tree, tree->root, 80, NumberCompare);
    TreeInsertNode(tree, tree->root, 100, NumberCompare);
    TreeInsertNode(tree, tree->root, 90, NumberCompare);
    TreeInsertNode(tree, tree->root, 95, NumberCompare);
    TreeInsertNode(tree, tree->root, 110, NumberCompare);

    TreeDtor(tree);

    return SUCCESS;
}