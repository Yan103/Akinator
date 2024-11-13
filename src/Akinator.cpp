#include <stdio.h>

#include "Akinator.h"
#include "Default.h"
#include "BinaryTree.h"
#include "TreeDump.h"
#include "FuncReturnCode.h"

const char* SAVEFILE = "/home/yan/projects/Akinator/WordBase/savedtree.txt";

FuncReturnCode StartAkinator(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n")

    printf(CYAN("Hi, I am an Akinator and I can guess the words that you have made up\n"));
    printf(YELLOW("What do you want:\n[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));

    char user_choice[MAX_USER_ANS] = {};

    while (scanf("%s", user_choice)) {
        if (strcasecmp(user_choice, "g") == 0) {
            StartAkinatorGuess(tree);

        } else if (strcasecmp(user_choice, "d") == 0) {
            printf(CYAN("Determining which object you want to get?\n"));

            char object[MAX_DATA_SIZE] = {};
            scanf("%499s", object);

            char** path = (char**) calloc(1, tree->size * sizeof(char**));
            if (!path) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }
            int* logic_path = (int*) calloc(1, tree->size * sizeof(int*));
            if (!logic_path) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }

            if (AkinatorGiveDefenition(tree, object, path, logic_path)) {
                printf(RED("No element in tree\n"));
            }

            FREE(path)
            FREE(logic_path)
            sleep(1);

        } else if (strcasecmp(user_choice, "c") == 0) {
            char  first_object[MAX_DATA_SIZE] = {};
            char second_object[MAX_DATA_SIZE] = {};

            printf(CYAN("Enter the 2 objects you want to compare:\n"));

            scanf("%499s", first_object);
            scanf("%499s", second_object);

            char** path1 = (char**) calloc(1, tree->size * sizeof(char**));
            if (!path1) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }
            int* logic_path1 = (int*) calloc(1, tree->size * sizeof(int*));
            if (!logic_path1) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }
            int first_res = TreeFindElem(tree->root, first_object, path1, logic_path1);

            char** path2 = (char**) calloc(1, tree->size * sizeof(char**));
            if (!path2) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }
            int* logic_path2 = (int*) calloc(1, tree->size * sizeof(int*));
            if (!logic_path2) {
                printf(RED("MEMORY ERROR!\n"));

                return MEMORY_ERROR;
            }

            int second_res = TreeFindElem(tree->root, second_object, path2, logic_path2);

            if (first_res + second_res != 2) {
                printf(RED("At least one object was not found in the tree\n"));
            } else {
                if (strcasecmp(first_object, second_object) == 0) {
                    printf(CYAN("The objects are the same\n"));
                } else {
                    printf(CYAN("Object %s differs from object %s in that:\n"), first_object, second_object);
                    AkinatorShowDifference(tree, first_object, path1, logic_path1, second_object, path2, logic_path2);

                    printf(CYAN("Object %s is similar to object %s in that:\n"), first_object, second_object);
                    AkinatorShowSimilarity(tree, first_object, path1, logic_path1, second_object, path2, logic_path2);
                }
            }

            FREE(path1) FREE(path2)
            FREE(logic_path1) FREE(logic_path2)
        } else if (strcasecmp(user_choice, "s") == 0) {
            int dump_id = 0;
            TREE_DUMP(tree, &dump_id, "%s", __func__)
            char command[MAX_DATA_SIZE] = {};

            sprintf(command, "eog /home/yan/projects/Akinator/DumpFiles/dump%d.png -f", dump_id);
            system(command);
        } else if (strcasecmp(user_choice, "e") == 0) {
            printf(YELLOW("A save exit has been triggered. See you soon)\n"));

            FILE* savefile = fopen(SAVEFILE, "w");
            if (!savefile) {
                printf(RED("FILE ERROR!\n"));

                return FILE_ERROR;
            }
            WriteTree(savefile, tree);

            break;
        } else if (strcasecmp(user_choice, "w") == 0) {
            printf(YELLOW("A call exit without saving. I hope we will play another game soon)\n"));

            break;
        } else {
            printf(RED("Please, use only the symbols available in the menu\n"));
            sleep(1);
        }
        printf(YELLOW("What do you want:\n[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));
    }

    return SUCCESS;
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

void StartAkinatorGuess(Tree* tree) {
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

int AkinatorGiveDefenition(Tree* tree, const char* find_data, char** path, int* logic_path) {
    ASSERT(tree       != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(find_data  != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(path       != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(logic_path != NULL, "NULL POINTER WAS PASSED!\n")

    if (TreeFindElem(tree->root, find_data, path, logic_path)) {
        printf(YELLOW("%s: "), find_data);
        for (size_t i = 0; i < tree->size - 1; i++) {
            if (path[i]) {
                if (logic_path[i] == 1) {
                    printf("%s ", path[i]);
                } else {
                    printf(RED("NO "));
                    printf("%s ", path[i]);
                }
                if (strcasecmp(path[i + 1], find_data) != 0) {printf("| ");}
                else {printf("\n"); break;}
            }
        }
        return 0;
    } else {
        return 1;
    }
}

FuncReturnCode AkinatorShowDifference(Tree* tree,
                                      char*  first_obj, char**  first_path, int*  first_logic_path,
                                      char* second_obj, char** second_path, int* second_logic_path) {
    ASSERT(tree              != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_obj         != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_path        != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_logic_path  != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_obj        != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_path       != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_logic_path != NULL, "NULL POINTER WAS PASSED!\n")

    for (size_t i = 0; i < tree->size - 1; i++) {
        if (strcasecmp(first_path[i], second_path[i]) != 0 || (first_logic_path[i] != second_logic_path[i])) {
            printf(YELLOW("%s: "), first_obj);
            if (first_logic_path[i] == -1) printf(RED("NO "));
            printf("%s\n", first_path[i]);

            printf(YELLOW("%s: "), second_obj);
            if (second_logic_path[i] == -1) printf(RED("NO "));
            printf("%s\n", second_path[i]);
            break;
        }
    }

    return SUCCESS;
}

FuncReturnCode AkinatorShowSimilarity(Tree* tree,
                                      char*  first_obj, char**  first_path, int*  first_logic_path,
                                      char* second_obj, char** second_path, int* second_logic_path) {
    ASSERT(tree              != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_obj         != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_path        != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(first_logic_path  != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_obj        != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_path       != NULL, "NULL POINTER WAS PASSED!\n")
    ASSERT(second_logic_path != NULL, "NULL POINTER WAS PASSED!\n")

    if (first_logic_path[0] != second_logic_path[0]) {
        printf(CYAN("The objects don't have anything similar\n"));

        return SUCCESS;
    }

    for (size_t i = 0; i < tree->size - 1; i++) {
        if (strcasecmp(first_path[i], second_path[i]) == 0 && (first_logic_path[i] == second_logic_path[i])) {
            if (first_logic_path[i] == -1) printf(RED("NO "));
            printf("%s\n", first_path[i]);
        } else {
            break;
        }
    }

    return SUCCESS;
}