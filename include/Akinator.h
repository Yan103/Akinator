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

FuncReturnCode StartAkinator(Tree* tree);

int AkinatorGiveDefenition(Tree* tree, const char* find_data, char** path, int* logic_path);

#endif // AKINATOR_H