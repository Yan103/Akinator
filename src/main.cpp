/*!
    \file
    File with the main function
*/

#include <stdio.h>
#include <time.h>

#include "Akinator.h"
#include "BinaryTree.h"
#include "Default.h"
#include "FuncReturnCode.h"

int main(const int argc, char* const *argv) {
    ASSERT(argv != NULL, "NULL PONTER WAS PASSED!\n");

    srand((unsigned int)time(NULL));

    const char* db_file = ReadCommandArgs(argc, argv);

    if (db_file) {
        FILE* database = fopen(db_file, "r");
        if (!database) {
            printf(RED("FILE ERROR!\n"));

            return FILE_ERROR;
        }
        Tree* tree = TreeCtor(ReadSubTree(database));

        fclose(database);

        StartAkinator(tree);

        TreeDtor(tree);
    } else {
        fprintf(stderr, RED("SOMETHING WENT WRONG...\n"));

        return UNKNOWN_ERROR;
    }

    return SUCCESS;
}
