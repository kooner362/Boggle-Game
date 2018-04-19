#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scoreboard.h"
#include "board.h"

//Display all users in linked list
void display_scores(Node **head){
    Node *current = *head;
    printf("------Scores------\n");
    while (current != NULL){
        printf("user name: %s\ntotal played: %d\n", current->name, current->total_games);
        printf("total score: %d\nbest score: %d \n\n", current->total_score, current->max_score);
        current = current->next;
    }
}

//Returns a Node pointer pointed at user Node.
Node* get_user(Node **head, char *name){
    Node *current;
    current = *head;
    while(current->next != NULL && strcmp(current->name, name) != 0){
        current = current->next;
    }
    return current;
}

//Adds user to the end of the linked list.
void add_user(Node **head, char *name, int max_score, int total_games, int total_score){
    Node *current;
    current = *head;
    if (strcmp(current->name, "null") == 0){
        strcpy(current->name, name);//use strcpy
        current->max_score = max_score;
        current->total_games = total_games;
        current->total_score = total_score;
        current->next = NULL;
    }
    else{
        Node *user = malloc(sizeof(Node));
        strcpy(user->name, name);//use strcpy
        user->max_score = max_score;
        user->total_games = total_games;
        user->total_score = total_score;
        user->next = NULL;

        while(current->next != NULL){
            current = current->next;
        }
        current->next = user; 
    }
     
}

//Updates the score of a user.
void update_user(Node **head, char *name, int score){
    Node *current;
    current = *head;
    while(strcmp(current->name, name) != 0){
        current = current->next;
    }
    if (current->max_score < score){
        current->max_score = score;
    }
    current->total_games++;
    current->total_score += score;  
}

//Checks whether a user exists or not. 0 mean user exists.
int existing_user(Node **head, char *name){
    Node *current;
    current = *head;
    int exists = 0;
    while(current->next != NULL && strcmp(current->name, name) !=0){
        current = current->next;
    }
    if (strcmp(current->name, name) == 0 && current != NULL){
        exists = 0;
    }
    else{
        exists = 1;
    }
    return exists;
}

//Create a linked list of users in memory from file.
void create_list(Node **head, char *filename){
    FILE *input;
    input = fopen(filename, "rb");
    if (input != NULL){
        
        *head = malloc(sizeof(Node));
        Node *current;
        current = *head;
        Node *next_node;
        fread(current, sizeof(Node), 1, input);
        
        while(!feof(input)){
            next_node = malloc(sizeof(Node));
            if (fread(next_node, sizeof(Node), 1, input) == 1){
                current->next = next_node;
                current = current->next;
            }
        }
        fclose(input);
   }
   else{
      *head = malloc(sizeof(Node));
      Node *current;
      current = *head;
      strcpy(current->name, "null");
   }
}

//Outputs the linked list of users to file.
void output_list(Node **head, char *filename){
    FILE *output;
    output = fopen(filename, "wb");
    Node *current;
    current = *head;
    while(current != NULL){
        fwrite(current, sizeof(Node), 1, output);
        current = current->next;
    }
    fclose(output);
    
}

//frees all memory allocated to linked list of users.
void free_linked_list(Node **head){
    Node *temp;
    Node *current;
    current = *head;
    while (current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
}