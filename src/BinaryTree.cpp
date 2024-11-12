#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>

#include "TreeDump.h"

Tree* TreeCtor(Node* root) {
    ASSERT(root != NULL, "NULL POINTER WAS PASSED!\n")

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (!tree) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = root;
    tree->size = 0;

    return tree;
}


Node* CreateNode(NodeData value) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        printf(RED("MEMORY ERROR!\n"));

        return NULL;
    }

    node->data  = value;

    return node;
}

int string_compare(const NodeData first_string, const NodeData secong_string) {
    ASSERT(first_string != NULL, "NULL PONTER WAS PASSED!\n")
    ASSERT(secong_string != NULL, "NULL PONTER WAS PASSED!\n")

    return strcasecmp(first_string, secong_string);
}

FuncReturnCode TreeInsertNode(Tree* tree, Node* node, NodeData value, CompType comp_func) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    TREE_DUMP(tree, "Start: %s (%s)", __func__, value)

    int comp_res = comp_func(value, node->data);

    if (comp_res <= 0) {
        if (node->left) {
            TreeInsertNode(tree, node->left, value, comp_func);
        } else {
            node->left = CreateNode(value);
        }
    } else {
        if (node->right) {
            TreeInsertNode(tree, node->right, value, comp_func);
        } else {
            node->right = CreateNode(value);
        }
    }

    tree->size += 1;

    TREE_DUMP(tree, "End: %s (%s)", __func__, value)

    return SUCCESS;
}

FuncReturnCode NodeDtor(Node* node) {
    FREE(node->data)
    if (node->left) {
        NodeDtor(node->left);
    }

    if (node->right) {
        NodeDtor(node->right);
    }

    FREE(node)

    return SUCCESS;
}

FuncReturnCode TreeDtor(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    NodeDtor(tree->root);
    FREE(tree)

    return SUCCESS;
}

size_t TreeNodesCount() {
    static size_t num = 0;
    num++;

    return num;
}

Node* ReadSubTree(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    int symbol = 0;

    //* Skip spaces
    while (isspace(symbol = fgetc(filename))) {
        ;
    }

    if (symbol == '*') {
        TreeNodesCount();

        return NULL;
    } else if (symbol != '{') {
        printf(RED("%s: unknown action symbol %c\n"), __func__, symbol);

        return NULL; //abort() ?
    }

    Node* node = CreateNode(ReadNodeData(filename)); // create node

    node->left  = ReadSubTree(filename);
    node->right = ReadSubTree(filename);

    while ((symbol = fgetc(filename)) != '}'){
        ;
    }

    return node;
}

char* ReadNodeData(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    char symbol = 0;
    while (isspace(symbol = fgetc(filename))){
        ;
    }

    if (symbol != '"') {
        printf(RED("ERROR WITH READ TREE FROM FILE1\n"));

        // abort() ?
    }

    char* node_data = (char*) calloc(1, MAX_DATA_SIZE * sizeof(char));
    if (!node_data) {
        printf(RED("MEMORY ERROR!\n"));

        // abort() ?
    }

    fscanf(filename, "%[^\"]", node_data);
    symbol = fgetc(filename);

    return node_data;
}

FuncReturnCode PlayGame(Tree* tree, Node* node) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n")

    char user_ans[MAX_USER_ANS] = {};

    node = AkinatorChoiceNode(node, user_ans);

    char answer[100] = {};

    system("espeak \"I think, it is...\"");
    sleep(2);
    sprintf(answer, "espeak \"%s\"", node->data);
    system(answer);
    printf("I think, it is... "),
    printf(GREEN("%s\n"), node->data);  //* test espeak system

    printf("Did I guess right? [yes/no]\n");

    AkinatorEndGame(tree, node, user_ans);

    return SUCCESS;
}

FuncReturnCode AkinatorAddUnknownWord(Tree* tree, Node* node) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n")

    char* new_word = (char*) calloc(1, MAX_DATA_SIZE * sizeof(char));
    if (!new_word) {
        printf(RED("MEMORY ERROR!\n"));

        return MEMORY_ERROR;
    }
    scanf("%s", new_word);

    printf("Please, write how %s differs from %s\n", new_word, node->data);

    char* new_question = (char*) calloc(1, MAX_DATA_SIZE * sizeof(char));
    if (!new_question) {
        printf(RED("MEMORY ERROR!\n"));

        return MEMORY_ERROR;
    }
    scanf("%s", new_question);

    node->left  = CreateNode(node->data);
    node->right = CreateNode(new_word);
    node->data  = new_question;

    tree->size += 1;

    return SUCCESS;
}

Node* AkinatorChoiceNode(Node* node, char* user_ans) {
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(user_ans != NULL, "NULL POINTER WAS PASSED!\n")

    while (node->left != NULL || node->right != NULL) {
        printf(YELLOW("%s? [yes/no]\n"), node->data);
        while (scanf("%s", user_ans)) {
            if (strcasecmp(user_ans, "yes") == 0 || strcasecmp(user_ans, "no") == 0) {
                break;
            } else {
                printf(RED("Please, answer only \"yes\" or \"no\")\n"));
                sleep(1);
                printf(YELLOW("%s? [yes/no]\n"), node->data);
            }
        }

        if (strcasecmp(user_ans, "yes") == 0) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    return node;
}

FuncReturnCode AkinatorEndGame(Tree* tree, Node* node, char* user_ans) {
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(user_ans != NULL, "NULL POINTER WAS PASSED!\n")

    while (scanf("%s", user_ans)) {
        if (strcasecmp(user_ans, "yes") == 0) {
            printf(GREEN("Oh yes, I won again!\n"));

            break;
        } else if (strcasecmp(user_ans, "no") == 0) {
            printf("Please, write the word that you have made up your mind\n");
            AkinatorAddUnknownWord(tree, node);
            printf(GREEN("Thank you, the new word has been successfully added now I know %lu sports!\n"), tree->size);

            break;
        } else {
            printf(RED("Please, answer only \"yes\" or \"no\")\n"));
            sleep(1);
            printf("Did I guess right?? [yes/no]\n");
        }
    }

    return SUCCESS;
}

void StartAkinatorGame(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")
    char start_game[50] = {};

    // TODO espeak voice || + time sleep
    printf(CYAN("Hello, I'm an akinator and I can guess the sport (I know %lu sports) that you guessed!\n"), tree->size);
    printf(CYAN("Do you want to start the game? [yes/no]\n"));

    while (scanf("%49s", start_game)) {
        if (strcasecmp(start_game, "yes") == 0) {
            PlayGame(tree, tree->root);

            while (1) {
                sleep(2);
                printf(MAGENTA("Do you want to play again? [yes/no]\n"));
                scanf("%49s", start_game);
                if (strcasecmp(start_game, "yes") == 0) {
                    PlayGame(tree, tree->root);
                } else if (strcasecmp(start_game, "no") == 0) {
                    printf(YELLOW("Okay, I'm waiting for our next game!\n"));
                    break;
                } else {
                    printf(RED("Please, answer only \"yes\" or \"no\")\n"));
                }
            }
            break;
        } else if (strcasecmp(start_game, "no") == 0) {
            printf(YELLOW("It's very sad, but I hope we'll play again!\n"));
            break;
        } else {
            printf(RED("Please, answer only \"yes\" or \"no\")\n"));
            printf(CYAN("Do you want to start the game? [yes/no]\n"));
        }
    }
}

FuncReturnCode WriteTree(FILE* filename, Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n")

    WriteSubTree(filename, tree->root);

    return SUCCESS;
}

FuncReturnCode WriteSubTree(FILE* filename, Node* node) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n")

    if (node == NULL) {
        fprintf(filename, "* ");

        return SUCCESS;
    }

    fprintf(filename, "{ ");

    fprintf(filename, "\"%s\" ", node->data);
    WriteSubTree(filename, node->left);
    WriteSubTree(filename, node->right);

    fprintf(filename, "} ");

    return SUCCESS;
}