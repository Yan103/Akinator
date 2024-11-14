#include <stdio.h>
#include <time.h>

#include "FuncReturnCode.h"
#include "BinaryTree.h"
#include "Default.h"
#include "Akinator.h"

#include "TreeDump.h"

// todo argc argv
const char* DATABASE = "/home/yan/projects/Akinator/WordBase/words.txt";

// TODO documentation and README
// TODO read filename from command line

int main() {
    srand((unsigned)time(NULL));

    FILE* database = fopen(DATABASE, "r");
    if (!database) {
        fprintf(stderr, RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }

    Tree* tree = TreeCtor(ReadSubTree(database));

    fclose(database);

    StartAkinator(tree);

    TreeDtor(tree);

    return SUCCESS;
}