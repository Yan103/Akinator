#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "BinaryTree.h"
#include "FuncReturnCode.h"

FuncReturnCode AkinatorAddUnknownWord(Tree* tree, Node* node);

Node* AkinatorChoiceNode(Node* node, char* user_ans);

FuncReturnCode AkinatorEndGame(Tree* tree, Node* node, char* user_ans);

FuncReturnCode PlayGame(Tree* tree, Node* node);

void StartAkinatorGuess(Tree* tree);
;
void StartAkinatorDefinition(Tree* tree);

void StartAkinatorCompare(Tree* tree);

FuncReturnCode StartAkinatorSaveExit(Tree* tree);

FuncReturnCode StartAkinator(Tree* tree);

FuncReturnCode StartAkinatorShowTree(Tree* tree);

int AkinatorGiveDefenition(Tree* tree, NodePath* node_path);

FuncReturnCode AkinatorShowDifference(Tree* tree, NodePath* obj1_path, NodePath* obj2_path);

FuncReturnCode AkinatorShowSimilarity(Tree* tree, NodePath* obj1_path, NodePath* obj2_path);

const char* ReadCommandArgs(const int argc, char* const *argv);

#endif // AKINATOR_H