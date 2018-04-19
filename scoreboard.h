#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdlib.h>

typedef struct node {
	char name[18];
	int max_score;
	int total_games;
	int total_score;
    struct node *next;
}Node;

void display_scores(Node **head);
Node* get_user(Node **head, char *name);
int existing_user(Node **head, char *name);
void add_user(Node **head, char *name, int max_score, int total_games, int total_score);
void update_user(Node **head, char *name, int score);
void free_linked_list(Node **head);
void create_list(Node **head, char *filename);
void output_list(Node **head, char *filename);

#endif