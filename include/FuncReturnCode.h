/*!
    \file
    File with return codes enum
*/

#ifndef FUNCRETURNCODE_H
#define FUNCRETURNCODE_H

/// @brief Enum with return functions codes
enum FuncReturnCode {
    SUCCESS         =  0,
    MEMORY_ERROR    = -1,
    FILE_ERROR      = -2,
    UNKNOWN_ERROR   = -3,
    TREE_READ_ERROR = -4,
    UNKNOWN_FLAG    = -5,
};

#endif // FUNCRETURNCODE_H