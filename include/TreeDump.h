#ifndef TREEDUMP_H
#define TREEDUMP_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "Default.h"
#include "FuncReturnCode.h"
#include "BinaryTree.h"

/// @brief Macro for the add new line in .dot file
#define NEWDOTLINE(filename) fprintf(filename, "\n  ");

/// @brief Macro for the add new line in .html file
#define NEWHTMLLINE(filename) fputc('\n', filename);

#ifdef DEBUG
    #define TREE_DUMP(tree, id, title, ...) { TreeDump(tree, __func__, __LINE__, id, title, __VA_ARGS__); }
#else
    #define TREE_DUMP(tree, id, title, ...) {}
#endif

/*!
    @brief Function that calls DUMP
    \param [out] tree   - pointer on tree
    \param  [in] func   - call function name
    \param  [in] line   - call function line
    \param  [in] title  - information about call function
*/
FuncReturnCode TreeDump(Tree* tree, const char* func, int line, int* id, const char* title, ...) __attribute__ ((format(printf, 5, 6)));

/*!
    @brief Function that creates base for DUMP
    \param [out] tree     - pointer on tree
    \param  [in] filename - filename .dot file for DUMP
*/
FuncReturnCode CreateDotBase(FILE* filename, Tree* tree);

FuncReturnCode CreateDotNode(FILE* filename, Node* node);

FuncReturnCode MakeHTMLDump(FILE* html_file, Tree* tree, int dump_id, const char* func, int line);

#endif // TREEDUMP_H