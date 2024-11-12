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
    //system("convert /home/yan/projects/Akinator/DumpFiles/dump1943778939.png -resize 60% /home/yan/projects/Akinator/DumpFiles/dump1943778939.png");
    //system("open /home/yan/projects/Akinator/DumpFiles/dump1943778939.png");
    srand((unsigned int)time(NULL));

    FILE* database = fopen(DATABASE, "r");
    if (!database) {
        printf(RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }

    Tree* tree = TreeCtor(ReadSubTree(database));
    tree->size = (TreeNodesCount() - 1) / 2;

    fclose(database);

    StartAkinator(tree);

    TreeDtor(tree);

    return SUCCESS;
}