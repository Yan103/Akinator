#include <stdio.h>
#include <time.h>

#include "FuncReturnCode.h"
#include "BinaryTree.h"
#include "Default.h"

#include "TreeDump.h"

const char* DATABASE = "/home/yan/projects/Akinator/WordBase/words.txt";
const char* SAVEFILE = "/home/yan/projects/Akinator/WordBase/savedtree.txt";

// TODO start function with description, rules and n-attempts to play (ask user about continue in the end of game)
// TODO documentation and README
// TODO ubrat' ves' crinzh iz coda
// TODO update dump (nodes with words -> another colour)

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

    TREE_DUMP(tree, "%s", __func__)

    FILE* savefile = fopen(SAVEFILE, "w");
    if (!savefile) {
        printf(RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }
    WriteTree(savefile, tree);

    TreeDtor(tree);

    return SUCCESS;
}