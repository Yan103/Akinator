#include <stdio.h>
#include <time.h>

#include "FuncReturnCode.h"
#include "BinaryTree.h"
#include "Default.h"

#include "TreeDump.h"

const char* DATABASE = "/home/yan/projects/Akinator/WordBase/words.txt";

int main() {
    FILE* database = fopen(DATABASE, "r");
    if (!database) {
        printf(RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }

    srand((unsigned int)time(NULL));

    Tree* tree = TreeCtor(ReadSubTree(database));

    fclose(database);

    TREE_DUMP(tree, "%s", __func__)

    PlayGame(tree->root);

    TreeDtor(tree);

    return SUCCESS;
}