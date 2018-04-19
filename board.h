#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "dictionary.h"
#include "scoreboard.h"
#include "board.h"

void display_board(char *letters);
void current_score(int *score, char *word);
int start_game(char *board_letters, DNode **dictionary, char* filename, Node **users);
int qu_checker(char *input, char *list, DNode **dictionary, DNode **submitted_d);
int word_checker(char *input, char *list, DNode **dictionary, DNode **submitted_d);
char* convert_upper(char* string);

#endif