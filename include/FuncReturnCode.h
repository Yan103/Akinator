#ifndef FUNCRETURNCODE_H
#define FUNCRETURNCODE_H

// #include <stdio.h>

enum FuncReturnCode {
    SUCCESS         =  0,
    MEMORY_ERROR    = -1,
    FILE_ERROR      = -2,
    UNKNOWN_ERROR   = -3,
    TREE_READ_ERROR = -4,
};

#endif // FUNCRETURNCODE_H