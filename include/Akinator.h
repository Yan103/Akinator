/*!
    \file
    File with Akinator's functions
*/

#ifndef AKINATOR_H
#define AKINATOR_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "BinaryTree.h"
#include "FuncReturnCode.h"

/// @brief Macro for the scanf buffer from the stdin (unknown length)
#define READ(buffer) { fgetc(stdin); scanf("%[^\n]", buffer); }

/*!
    @brief Function that launces the Akinator
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinator(Tree* tree);

/*!
    @brief Function that starts Akinator's game
    \param [in] tree - pointer on tree
    \param [in] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode PlayGame(Tree* tree, Node* node);

/*!
    @brief Function that starts the Akinator guess mode
    \param [in] tree - pointer on tree
*/
void StartAkinatorGuess(Tree* tree);

/*!
    @brief Function that changes the current node depending on the user's response
    \param [out] node     - pointer on node
    \param  [in] user_ans - user's answer
    @return The pointer on the selected node
*/
Node* AkinatorChoiceNode(Node* node, char* user_ans);

/*!
    @brief Function that ends Akinator's guess mode
    \param [in] tree     - pointer on tree
    \param [in] node     - pointer on node
    \param [in] user_ans - user's answer
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorEndGame(Tree* tree, Node* node, char* user_ans);

/*!
    @brief Function that adds new (unknown word) in the tree
    \param  [in] tree - pointer on tree
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorAddUnknownWord(Tree* tree, Node* node);

/*!
    @brief Function that starts Akinator give defenition mode
    \param [in] tree - pointer on tree
*/
void StartAkinatorDefinition(Tree* tree);

/*!
    @brief Function that definse the object
    \param [in] tree - pointer on tree
    \param [in] node - pointer on node
    @return 0 if it was possible to find such an element and give a definition, otherwise 1
*/
int AkinatorGiveDefinition(Tree* tree, NodePath* node_path);

/*!
    @brief Function that starts Akinator compare mode
    \param  [in] tree - pointer on tree
*/
void StartAkinatorCompare(Tree* tree);

/*!
    @brief Function that starts Akinator show difference mode
    \param [in] tree      - pointer on tree
    \param [in] obj1_path - pointer on struct NodePath (first objects)
    \param [in] obj2_path - pointer on struct NodePath (second objects)
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorShowDifference(Tree* tree, NodePath* obj1_path, NodePath* obj2_path);

/*!
    @brief Function that starts Akinator show similarity mode
    \param [in] tree      - pointer on tree
    \param [in] obj1_path - pointer on struct NodePath (first objects)
    \param [in] obj2_path - pointer on struct NodePath (second objects)
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorShowSimilarity(Tree* tree, NodePath* obj1_path, NodePath* obj2_path);

/*!
    @brief Function that launches Akinator exit with saving
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinatorSaveExit(Tree* tree);

/*!
    @brief Function that starts the Akinator show tree mode
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinatorShowTree(Tree* tree);

/*!
    @brief Function that reads command line arguments for read filename
    \param [in] argc - argument count
    \param [in] argv - pointer on argument values
    @return The database filename
*/
const char* ReadCommandArgs(const int argc, char* const *argv);

#endif // AKINATOR_H