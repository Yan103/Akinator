/*!
    \file
    File with binary tree DUMP functions
*/

#include <stdio.h>

#include "TreeDump.h"

/// @brief Constant for LOG filename
const char* LOG_FILENAME            = "/home/yan/projects/Akinator/DumpFiles/log.html";

/// @brief Constant for DOT filename
const char* DOT_FILENAME            = "/home/yan/projects/Akinator/DumpFiles/dump.dot";

/// @brief Constant for command busser size
const int   COMMAND_BUFFER_CONSTANT = 500;

/*!
    @brief Function that calls DUMP
    \param [out] tree   - pointer on tree
    \param  [in] func   - call function name
    \param  [in] line   - call function line
    \param  [in] title  - information about call function
    @return The DUMP id
*/
int TreeDump(Tree* tree, const char* func, int line, const char* title, ...) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    FILE* dot_file = fopen(DOT_FILENAME, "w");
    if (!dot_file) {
        printf(RED("Error occured while opening input file!\n"));

        return -1;
    }

    CreateDotBase(dot_file, tree);
    CreateDotNode(dot_file, tree->root);
    fprintf(dot_file, "\n}");

    int dump_id = rand();

    char* command = (char*) calloc(COMMAND_BUFFER_CONSTANT, sizeof(char));
    if (command == NULL) {
        printf(RED("MEMORY ERROR!\n"));

        return -2;
    }
    fclose(dot_file);

    sprintf(command, "dot -Tpng %s -o /home/yan/projects/Akinator/DumpFiles/dump%d.png",
                                 DOT_FILENAME,                                   dump_id);

    int system_end = system(command);
    if (system_end != 0) {
        printf(RED("Something went wrong...\n"));
        FREE(command)

        return -3;
    }
    FREE(command);

    FILE* html_file = fopen(LOG_FILENAME, "a");
    if (!html_file) {
        printf(RED("Error occured while opening input file!\n"));

        return -1;
    }

    fprintf(html_file, "<pre>\n<hr>\n    <font size=\"10\">");

    va_list args = {};
    va_start(args, title);

    vfprintf(html_file, title, args);

    va_end(args);

    fprintf(html_file, " ✅</font>\n    ");

    MakeHTMLDump(html_file, tree, dump_id, func, line);
    fclose(html_file);

    return dump_id;
}

/*!
    @brief Function that returns time in what function was launched
    @return The information about the current time
*/
static tm GetTime() {
    time_t time_now = time(NULL);

    return *localtime(&time_now);
}

/*!
    @brief Function that creates base for DUMP
    \param [in] filename - filename .dot file for DUMP
    \param [in]     tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode CreateDotBase(FILE* filename, Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    fprintf(filename, "digraph tree{\n    node[shape=record,fontsize=14];\n    splines=ortho\n    ");
    fprintf(filename, "info[label=\"root=%p\"]\n", tree->root);
    NEWDOTLINE(filename);

    return SUCCESS;
}

/*!
    @brief Function that creates node in DUMP
    \param [in] filename - filename .dot file for DUMP
    \param [in]     node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode CreateDotNode(FILE* filename, Node* node) {
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    if (node->left) {
        fprintf(filename, "\tnode%p[shape=Mrecord,style=\"rounded,filled\",fillcolor=\"lightgreen\","
                          "label=\"{ %s }\"]\n", node, node->data);
        fprintf(filename, "\tnode%p->node%p[xlabel=\"No\"]\n", node, node->left);
        CreateDotNode(filename, node->left);
    } else {
        fprintf(filename, "\tnode%p[shape=Mrecord,style=\"rounded,filled\",fillcolor=\"lightpink\","
                          "label=\"{ %s }\"]\n", node, node->data);
    }

    if (node->right) {
        fprintf(filename, "\tnode%p[shape=Mrecord,style=\"rounded,filled\",fillcolor=\"lightgreen\","
                          "label=\"{ %s }\"]\n", node, node->data);
        fprintf(filename, "\tnode%p->node%p[xlabel=\"Yes\"]\n", node, node->right);
        CreateDotNode(filename, node->right);
    } else {
        fprintf(filename, "\tnode%p[shape=Mrecord,style=\"rounded,filled\",fillcolor=\"lightpink\","
                          "label=\"{ %s }\"]\n", node, node->data);
    }

    return SUCCESS;
}

/*!
    @brief Function that creates HTML file with DUMP
    \param [in] filename - filename .html file for DUMP
    \param [in]     tree - pointer on tree
    \param [in]  dump_id - dump id
    \param [in]     func - call function name
    \param [in]     line - call function line
    @return The status of the function (return code)
*/
FuncReturnCode MakeHTMLDump(FILE* html_file, Tree* tree, int dump_id, const char* func, int line) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(html_file != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(func      != NULL, "NULL POINTER WAS PASSED!\n");

    struct tm tm = GetTime();

    fprintf(html_file, "DUMPED %d-%02d-%02d %02d:%02d:%02d from function %s, %d line \n    ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, func, line);
    fprintf(html_file, "Tree size: %lu nodes\n", tree->size);

    fprintf(html_file, "\t<img src=dump%d.png>", dump_id);

    return SUCCESS;
}