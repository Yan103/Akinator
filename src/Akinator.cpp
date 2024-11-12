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

            printf(YELLOW("What do you want:\n[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));
        } else if (strcasecmp(user_choice, "d") == 0) {
            printf("in development...");                    //! dodelay
        } else if (strcasecmp(user_choice, "c") == 0) {
            printf("in development...");                    //! dodelay
        } else if (strcasecmp(user_choice, "s") == 0) {
            TREE_DUMP(tree, "%s", __func__)

            //system("convert /home/yan/projects/Akinator/DumpFiles/dump1943778939.png -resize 60% /home/yan/projects/Akinator/DumpFiles/dump1943778939.png");
            system("open /home/yan/projects/Akinator/DumpFiles/dump1943778939.png"); //! что делать с уникальным номером?

            printf(YELLOW("What do you want:\n[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?\n"));
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
            printf(YELLOW("A call without saving. I hope we will play another game soon)\n"));

            break;
        } else {
            printf(RED("Please, use only the symbols available in the menu\n"));
            sleep(1);
            printf(YELLOW("What do you want:\n[G]uess, [D]efine, [C]ompare objects, [S]how the tree, [E]xit with or [w]ithout saving?"));
        }
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