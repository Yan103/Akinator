#include <stdio.h>

#include "TreeDump.h"

const char* LOG_FILENAME            = "/home/yan/projects/Akinator/DumpFiles/log.html";
const char* DOT_FILENAME            = "/home/yan/projects/Akinator/DumpFiles/dump.dot";
const int   COMMAND_BUFFER_CONSTANT = 500;

FuncReturnCode TreeDump(Tree* tree, const char* func, int line, const char* title, ...) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    FILE* dot_file = fopen(DOT_FILENAME, "w");
    if (!dot_file) {
        printf(RED("Error occured while opening input file!\n"));

        return FILE_ERROR;
    }

    CreateDotBase(dot_file, tree);
    CreateDotNode(dot_file, tree->root);
    fprintf(dot_file, "\n}");

    int dump_id = rand();

    char* command = (char*) calloc(COMMAND_BUFFER_CONSTANT, sizeof(char));
    if (command == NULL) {
        printf(RED("MEMORY ERROR!\n"));

        return MEMORY_ERROR;
    }
    fclose(dot_file);

    sprintf(command, "dot -Tpng %s -o /home/yan/projects/Akinator/DumpFiles/dump%d.png",
                                 DOT_FILENAME,                                   dump_id);

    int system_end = system(command);
    if (system_end != 0) {
        printf(RED("Something went wrong...\n"));
        FREE(command)

        return UNKNOWN_ERROR;
    }
    FREE(command)

    FILE* html_file = fopen(LOG_FILENAME, "a");
    if (!html_file) {
        printf(RED("Error occured while opening input file!\n"));

        return FILE_ERROR;
    }

    fprintf(html_file, "<pre>\n<hr>\n    <font size=\"10\">");

    va_list args = {};
    va_start(args, title);

    vfprintf(html_file, title, args);

    va_end(args);

    fprintf(html_file, " ✅</font>\n    ");

    MakeHTMLDump(html_file, tree, dump_id, func, line);
    fclose(html_file);

    return SUCCESS;
}

/*!
    @brief Function that returns time in what function was launched
*/
static tm GetTime() {
    time_t time_now = time(NULL);

    return *localtime(&time_now);
}

/*!
    @brief Function that creates base for DUMP
    \param [out] tree     - pointer on tree
    \param  [in] filename - filename .dot file for DUMP
*/
FuncReturnCode CreateDotBase(FILE* filename, Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    fprintf(filename, "digraph tree{\n    node[shape=record,fontsize=14];\n    splines=ortho\n    ");
    fprintf(filename, "info[label=\"root=%p\"]\n", tree->root);
    NEWDOTLINE(filename)

    return SUCCESS;
}

FuncReturnCode CreateDotNode(FILE* filename, Node* node) {
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    fprintf(filename, "\tnode%p[shape=Mrecord,style=\"rounded,filled\",fillcolor=\"lightgreen\",label=\"{ %s }\"]\n", node, node->data);
    if (node->left) {
        fprintf(filename, "\tnode%p->node%p[xlabel=\"No\"]\n", node, node->left);
        CreateDotNode(filename, node->left);
    }

    if (node->right) {
        fprintf(filename, "\tnode%p->node%p[xlabel=\"Yes\"]\n", node, node->right);
        CreateDotNode(filename, node->right);
    }

    return SUCCESS;
}

FuncReturnCode MakeHTMLDump(FILE* html_file, Tree* tree, int dump_id, const char* func, int line) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(html_file != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(func      != NULL, "NULL POINTER WAS PASSED!\n")

    struct tm tm = GetTime();

    fprintf(html_file, "DUMPED %d-%02d-%02d %02d:%02d:%02d from function %s, %d line \n    ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, func, line);
    fprintf(html_file, "Tree size: %lu nodes\n", tree->size);

    fprintf(html_file, "\t<img src=dump%d.png>", dump_id);

    return SUCCESS;
}