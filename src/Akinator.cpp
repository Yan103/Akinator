/*!
    \file
    File with Akinator's functions
*/

#include <stdio.h>

#include "Akinator.h"
#include "BinaryTree.h"
#include "Default.h"
#include "FuncReturnCode.h"
#include "TreeDump.h"

/// @brief Constant for default database savefile
const char* SAVEFILE        = "/home/yan/projects/Akinator/WordBase/savedtree.txt";

/// @brief Constant for default database file
const char* DATABASE        = "/home/yan/projects/Akinator/WordBase/words.txt";

/// @brief Constant for show binary tree on the screen
const char* SHOW_TREE_CONST = "eog /home/yan/projects/Akinator/DumpFiles/dump";

/*!
    @brief Function that create NodePath struct for the node
    \param [in] data - pointer on node data
    \param [in] size - count of nodes
    @return The pointer on NodePath struct
*/
static NodePath* NodePathInit(NodeData data, size_t size) {
    ASSERT(data != NULL, "NULL POINTER WAS PASSED!\n");

    NodePath* node_path = (NodePath*) calloc(1, sizeof(NodePath));
    if (!node_path) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }
    node_path->data = data;

    NodeData* path = (NodeData*) calloc(size, sizeof(NodePath));
    if (!path) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }
    node_path->path = path;

    int* logic_path = (int*) calloc(size, sizeof(int));
    if (!logic_path) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }
    node_path->logic_path = logic_path;

    return node_path;
}

/*!
    @brief Function that clean NodePath struct
    \param [out] node_path - pointer on NodePath struct
*/
static void NodePathClean(NodePath* node_path) {
    ASSERT(node_path != NULL, "NULL POINTER WAS PASSED!\n");

    FREE(node_path->logic_path);
    FREE(node_path->path);
    FREE(node_path);
}

/*!
    @brief Function that launces the Akinator
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinator(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    printf(CYAN("Hi, I am an Akinator and I can guess the words that you have made up\n"));
    printf(YELLOW("What do you want:\n"
                  "[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));

    char user_choice[MAX_USER_ANS] = {};

    while (scanf("%[^\n]", user_choice)) {
        if (strcasecmp(user_choice, "g") == 0) {
            StartAkinatorGuess(tree);

        } else if (strcasecmp(user_choice, "d") == 0) {
            StartAkinatorDefinition(tree);

        } else if (strcasecmp(user_choice, "c") == 0) {
            StartAkinatorCompare(tree);

        } else if (strcasecmp(user_choice, "s") == 0) {
            StartAkinatorShowTree(tree);

        } else if (strcasecmp(user_choice, "e") == 0) {
            StartAkinatorSaveExit(tree);

            break;
        } else if (strcasecmp(user_choice, "w") == 0) {
            printf(YELLOW("A call exit without saving. I hope we will play another game soon)\n"));

            break;
        } else {
            printf(RED("Please, use only the symbols available in the menu\n"));
            sleep(1);
        }
        printf(YELLOW("What do you want:\n"
                      "[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));
        fgetc(stdin);
    }

    return SUCCESS;
}

/*!
    @brief Function that starts Akinator's game
    \param [in] tree - pointer on tree
    \param [in] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode PlayGame(Tree* tree, Node* node) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    char user_ans[MAX_USER_ANS] = {};

    node = AkinatorChoiceNode(node, user_ans);

    char answer[MAX_DATA_SIZE] = {};

    if (system("espeak \"I think, it is...\"")) {
        fprintf(stderr, RED("Something went wrong...\n"));

        return UNKNOWN_ERROR;
    }
    sleep(2);
    sprintf(answer, "espeak \"%s\"", node->data);
    if (system(answer)) {
        fprintf(stderr, RED("Something went wrong...\n"));

        return UNKNOWN_ERROR;
    }
    printf("I think, it is... "),
    printf(GREEN("%s\n"), node->data);  //* test espeak system

    printf("Did I guess right? [yes/no]\n");

    AkinatorEndGame(tree, node, user_ans);

    return SUCCESS;
}

/*!
    @brief Function that adds new (unknown word) in the tree
    \param  [in] tree - pointer on tree
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorAddUnknownWord(Tree* tree, Node* node) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    char* new_word = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    if (!new_word) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return MEMORY_ERROR;
    }
    READ(new_word);

    printf("Please, write how \"%s\" differs from \"%s\"\n", new_word, node->data);

    char* new_question = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    if (!new_question) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return MEMORY_ERROR;
    }
    READ(new_question);

    node->left  = CreateNode(node->data);
    node->right = CreateNode(new_word);
    node->data  = new_question;

    tree->size += 1;

    return SUCCESS;
}

/*!
    @brief Function that changes the current node depending on the user's response
    \param [out] node     - pointer on node
    \param  [in] user_ans - user's answer
    @return The pointer on the selected node
*/
Node* AkinatorChoiceNode(Node* node, char* user_ans) {
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(user_ans != NULL, "NULL POINTER WAS PASSED!\n");

    fgetc(stdin);
    while (node->left != NULL || node->right != NULL) {
        printf(YELLOW("%s? [yes/no]\n"), node->data);
        while (scanf("%[^\n]", user_ans)) {
            if (strcasecmp(user_ans, "yes") == 0 || strcasecmp(user_ans, "no") == 0) {
                break;
            } else {
                printf(RED("Please, answer only \"yes\" or \"no\")\n"));
                sleep(1);
                printf(YELLOW("%s? [yes/no]\n"), node->data);
            }
            fgetc(stdin);
        }

        if (strcasecmp(user_ans, "yes") == 0) {
            node = node->right;
        } else {
            node = node->left;
        }
        fgetc(stdin);
    }

    return node;
}

/*!
    @brief Function that ends Akinator's guess mode
    \param [in] tree     - pointer on tree
    \param [in] node     - pointer on node
    \param [in] user_ans - user's answer
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorEndGame(Tree* tree, Node* node, char* user_ans) {
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(node     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(user_ans != NULL, "NULL POINTER WAS PASSED!\n");

    while (scanf("%[^\n]", user_ans)) {
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
        fgetc(stdin);
    }

    return SUCCESS;
}

/*!
    @brief Function that starts the Akinator guess mode
    \param [in] tree - pointer on tree
*/
void StartAkinatorGuess(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    char start_game[MAX_USER_ANS] = {};

    printf(CYAN("Hello, I'm an akinator and I can guess the sport (I know %lu sports) that you guessed!\n"), tree->size);
    printf(CYAN("Do you want to start the game? [yes/no]\n"));

    fgetc(stdin);
    while (scanf("%[^\n]", start_game)) {
        if (strcasecmp(start_game, "yes") == 0) {
            PlayGame(tree, tree->root);

            while (1) {
                sleep(2);
                printf(MAGENTA("Do you want to play again? [yes/no]\n"));
                fgetc(stdin);
                scanf("%[^\n]", start_game);
                if (strcasecmp(start_game, "yes") == 0) {
                    PlayGame(tree, tree->root);
                } else if (strcasecmp(start_game, "no") == 0) {
                    printf(YELLOW("Okay, I'm waiting for our next game!\n"));
                    break;
                } else {
                    printf(RED("Please, answer only \"yes\" or \"no\")\n"));
                    fgetc(stdin);
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
        fgetc(stdin);
    }
    sleep(1);
}

/*!
    @brief Function that starts Akinator give defenition mode
    \param [in] tree - pointer on tree
*/
void StartAkinatorDefinition(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    printf(CYAN("Determining which object you want to get?\n"));

    char object[MAX_DATA_SIZE] = {};
    READ(object);

    NodePath* obj_path = NodePathInit(object, tree->size);

    if (AkinatorGiveDefinition(tree, obj_path)) {
        fprintf(stderr, RED("No element in tree\n"));
    }

    NodePathClean(obj_path);
    sleep(1);
}

/*!
    @brief Function that definse the object
    \param [in] tree - pointer on tree
    \param [in] node - pointer on node
    @return 0 if it was possible to find such an element and give a definition, otherwise 1
*/
int AkinatorGiveDefinition(Tree* tree, NodePath* node_path) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(node_path != NULL, "NULL POINTER WAS PASSED!\n");

    if (TreeFindElem(tree->root, node_path->data, node_path->path, node_path->logic_path)) {
        printf(YELLOW("%s: "), node_path->data);
        for (size_t i = 0; i < tree->size - 1; i++) {
            if (node_path->path[i]) {
                if (node_path->logic_path[i] == 1) {
                    printf("%s ", node_path->path[i]);
                } else {
                    printf(RED("NO ") "%s ", node_path->path[i]);
                }
                if (strcasecmp(node_path->path[i + 1], node_path->data) != 0) {
                    printf("| ");
                }
                else {
                    printf("\n");
                    break;
                }
            }
        }
        return 0;
    } else {
        return 1;
    }
}

/*!
    @brief Function that shows the difference
    \param [in] object     - object's data
    \param [in] no_label   - logic path value
    \param [in] difference - pointer on the difference value
*/
static void ShowDifference(char* object, int no_label, char* difference) {
    ASSERT(object     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(difference != NULL, "NULL POINTER WAS PASSED!\n");

    printf(YELLOW("%s: "), object);
    if (no_label == -1) printf(RED("NO "));
    printf("%s\n", difference);
}

/*!
    @brief Function that starts Akinator show difference mode
    \param [in] tree      - pointer on tree
    \param [in] obj1_path - pointer on struct NodePath (first objects)
    \param [in] obj2_path - pointer on struct NodePath (second objects)
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorShowDifference(Tree* tree, NodePath* obj1_path, NodePath* obj2_path) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj1_path != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj2_path != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < tree->size - 1; i++) {
        if (strcasecmp(obj1_path->path[i], obj2_path->path[i]) != 0
            || (obj1_path->logic_path[i] != obj2_path->logic_path[i])) {

            ShowDifference(obj1_path->data, obj1_path->logic_path[i], obj1_path->path[i]);
            ShowDifference(obj2_path->data, obj2_path->logic_path[i], obj2_path->path[i]);

            break;
        }
    }

    return SUCCESS;
}

/*!
    @brief Function that starts Akinator show similarity mode
    \param [in] tree      - pointer on tree
    \param [in] obj1_path - pointer on struct NodePath (first objects)
    \param [in] obj2_path - pointer on struct NodePath (second objects)
    @return The status of the function (return code)
*/
FuncReturnCode AkinatorShowSimilarity(Tree* tree, NodePath* obj1_path, NodePath* obj2_path) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj1_path != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj2_path != NULL, "NULL POINTER WAS PASSED!\n");

    if (obj1_path->logic_path[0] != obj2_path->logic_path[0]) {
        printf(CYAN("The objects don't have anything similar\n"));

        return SUCCESS;
    }

    for (size_t i = 0; i < tree->size - 1; i++) {
        if (strcasecmp(obj1_path->path[i], obj2_path->path[i]) == 0
            && (obj1_path->logic_path[i] == obj2_path->logic_path[i])) {
            if (obj1_path->logic_path[i] == -1) printf(RED("NO "));
            printf("%s\n", obj1_path->path[i]);
        } else {
            break;
        }
    }

    return SUCCESS;
}

/*!
    @brief Function that reads command line arguments for read filename
    \param [in] argc - argument count
    \param [in] argv - pointer on argument values
    @return The database filename
*/
const char* ReadCommandArgs(const int argc, char* const *argv) {
    ASSERT(argv != NULL, "NULL POINTER WAS PASSED!\n");

    int       opt = 0;
    int have_args = 0;

    while((opt = getopt(argc, argv, "f")) != -1) {
        have_args = 1;
        switch (opt) {
            case 'f':
                if (argc == 3) {
                    return argv[2];
                } else {
                    printf(RED("Your file has not been found! The standard text file (%s) is launched:\n"),
                                                                                      DATABASE);
                    return DATABASE;
                }
                break;
            default:
                fprintf(stderr, RED("Flag error!\n"));
                return NULL;
        }
    }

    if (!have_args) {
        printf(BLUE("Command line arguments are not received, default file (%s) are launched\n"), DATABASE);
        return DATABASE;
    }

    return NULL;
}

/*!
    @brief Function that launches Akinator exit with saving
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinatorSaveExit(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    printf(YELLOW("A save exit has been triggered. See you soon)\n"));

    FILE* savefile = fopen(SAVEFILE, "w");
    if (!savefile) {
        fprintf(stderr, RED("FILE ERROR!\n"));

        return FILE_ERROR;
    }
    WriteTree(savefile, tree);
    fclose(savefile);

    return SUCCESS;
}

/*!
    @brief Function that starts the Akinator show tree mode
    \param [in] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode StartAkinatorShowTree(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    char command[MAX_DATA_SIZE] = {};
    int dump_id = TreeDump(tree, __func__, __LINE__, "%s", __func__);

    sprintf(command, "%s%d.png -f", SHOW_TREE_CONST, dump_id);

    if (system(command)) {
        fprintf(stderr, RED("Something went wrong...\n"));

        return UNKNOWN_ERROR;
    }

    return SUCCESS;
}
/*!
    @brief Function that starts Akinator compare mode
    \param [in]       tree - pointer on tree
    \param [in]  first_res - first result of compare function
    \param [in] second_res - second result of compare function
    \param [in]  obj1_path - pointer on struct NodePath (first objects)
    \param [in]  obj2_path - pointer on struct NodePath (second objects)
*/
static void ShowCompareResult(Tree* tree, int first_res, int second_res, NodePath* obj1_path, NodePath* obj2_path) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj1_path != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(obj2_path != NULL, "NULL POINTER WAS PASSED!\n");

    if (first_res + second_res != 2) {
        printf(RED("At least one object was not found in the tree\n"));
    } else {
        if (strcasecmp(obj1_path->data, obj2_path->data) == 0) {
            printf(CYAN("The objects are the same\n"));
        } else {
            printf(CYAN("Object \"%s\" differs from object \"%s\" in that:\n"), obj1_path->data, obj2_path->data);
            AkinatorShowDifference(tree, obj1_path, obj2_path);

            printf(CYAN("Object \"%s\" is similar to object \"%s\" in that:\n"), obj1_path->data, obj2_path->data);
            AkinatorShowSimilarity(tree, obj1_path, obj2_path);
        }
    }
}

/*!
    @brief Function that starts Akinator compare mode
    \param  [in] tree - pointer on tree
*/
void StartAkinatorCompare(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    char  first_object[MAX_DATA_SIZE] = {};
    char second_object[MAX_DATA_SIZE] = {};

    printf(CYAN("Enter the 2 objects you want to compare:\n"));

    READ(first_object);
    READ(second_object);

    NodePath* obj1_path = NodePathInit(first_object, tree->size);
    int first_res = TreeFindElem(tree->root, obj1_path->data, obj1_path->path, obj1_path->logic_path);

    NodePath* obj2_path = NodePathInit(second_object, tree->size);
    int second_res = TreeFindElem(tree->root, obj2_path->data, obj2_path->path, obj2_path->logic_path);

    ShowCompareResult(tree, first_res, second_res, obj1_path, obj2_path);

    NodePathClean(obj1_path);
    NodePathClean(obj2_path);

    sleep(1);
}
