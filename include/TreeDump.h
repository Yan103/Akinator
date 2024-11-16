/*!
    \file
    File with binary tree DUMP functions
*/

#ifndef TREEDUMP_H
#define TREEDUMP_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "BinaryTree.h"
#include "Default.h"
#include "FuncReturnCode.h"

/// @brief Macro for the add new line in .dot file
#define NEWDOTLINE(filename) fprintf(filename, "\n  ");

/// @brief Macro for the add new line in .html file
#define NEWHTMLLINE(filename) fputc('\n', filename);

#ifdef DEBUG
    #define TREE_DUMP(tree, title, ...) { TreeDump(tree, __func__, __LINE__, title, __VA_ARGS__); }
#else
    #define TREE_DUMP(tree, title, ...) {}
#endif

/*!
    @brief Function that calls DUMP
    \param [out] tree   - pointer on tree
    \param  [in] func   - call function name
    \param  [in] line   - call function line
    \param  [in] title  - information about call function
    @return The DUMP id
*/
int TreeDump(Tree* tree, const char* func, int line, const char* title, ...) __attribute__ ((format(printf, 4, 5)));

/*!
    @brief Function that creates base for DUMP
    \param [in] filename - filename .dot file for DUMP
    \param [in]     tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode CreateDotBase(FILE* filename, Tree* tree);

/*!
    @brief Function that creates node in DUMP
    \param [in] filename - filename .dot file for DUMP
    \param [in]     node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode CreateDotNode(FILE* filename, Node* node);

/*!
    @brief Function that creates HTML file with DUMP
    \param [in] filename - filename .html file for DUMP
    \param [in]     tree - pointer on tree
    \param [in]  dump_id - dump id
    \param [in]     func - call function name
    \param [in]     line - call function line
    @return The status of the function (return code)
*/
FuncReturnCode MakeHTMLDump(FILE* html_file, Tree* tree, int dump_id, const char* func, int line);

#endif // TREEDUMP_H