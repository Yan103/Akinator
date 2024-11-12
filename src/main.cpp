#include <stdio.h>
#include <time.h>

#include "FuncReturnCode.h"
#include "BinaryTree.h"
#include "Default.h"
#include "Akinator.h"

#include "TreeDump.h"

const char* DATABASE = "/home/yan/projects/Akinator/WordBase/words.txt";

// TODO documentation and README
// TODO Definition, difference, Similarities, Graph Dump

int main() {
    srand((unsigned int)time(NULL));

    FILE* database = fopen(DATABASE, "r");
    if (!database) {
        printf(RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }

    Tree* tree = TreeCtor(ReadSubTree(database));
    tree->size = (TreeNodesCount() - 1) / 2;

    fclose(database);

    //* tests
    NodeData  path[100] = {};
    int      path1[100] = {};
    TreeFindElem(tree->root, "Baseball", path, path1);

    for (int i = 0; i < 100; i++) {
        if (strcasecmp(path[i], "Baseball") == 0) break;

        if (path[i]) {
            printf("%d %s\n", path1[i], path[i]);
        }

    } // TODO vse cherez odin massiv

    //StartAkinator(tree);

    TreeDtor(tree);

    return SUCCESS;
}