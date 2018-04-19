#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"
#include "dictionary.h"
#include "scoreboard.h"
#include "board.h"
#include <ctype.h>

/***
Converts a string to lower case. 
***/
char* convert_lower(char* string){
    int i;
    char *lower = malloc((sizeof(char)*strlen(string)) + 1);
    for (i = 0; i < strlen(string); i++){
        lower[i] = (char) tolower(string[i]); 
    }
    lower[strlen(string)] = '\0';
    return lower;
}

/***
Converts a string to upper case. 
***/
char* convert_upper(char* string){
    int i;
    char *upper = malloc((sizeof(char)*strlen(string)) + 1);
    for (i = 0; i < strlen(string); i++){
        upper[i] = (char) toupper(string[i]); 
    }
    upper[strlen(string)] = '\0';
    return upper;
}

/***
Updates the current score of a user based on the length of the word.
***/
void current_score(int *score_ptr, char *word){
    if (strlen(word) > 2 && strlen(word) < 5){
        *score_ptr += 1;
    }
    else if(strlen(word) == 5){
        *score_ptr += 2;
    }
    else if(strlen(word) == 6){
        *score_ptr += 3;
    }
    else if (strlen(word) == 7){
        *score_ptr += 5;
    }
    else if(strlen(word) > 7){
        *score_ptr += 11;
    } 
}

/***
Display a 4x4 grid with letters across the board.
***/
void display_board(char *letters){
    system("clear");
    int i;
    printf("--------------------\n");
    for (i = 0; i < 16; i++){
        
        if ((i - 3) % 4 != 0){
            printf("| ");
            printf("%c", letters[i]);
            printf(" |");
        }
        else{
            printf("| ");
            printf("%c", letters[i]);
            printf(" |");
            printf("\n--------------------\n");
        }
        
        
    }
}

/***
Returns all possible directions in which a user can traverse on the board
***/
void possible_directions(int index, int **output_list){
    *output_list = malloc(sizeof(int) * 8);
    int i = 0;
    int *list = *output_list;
    while (i != 8){
	if ((index + 1) < 16 && i < 1 && (((index + 1) % 4) !=0)){
	    list[i] = (index + 1);
	    i++;
    }
	else if ((index - 1) > -1 && i < 2 && i > 0 && ((index % 4) !=0)){
	    list[i] = (index - 1);
	    i++;
	}
	else if ((index + 4) < 16 && i < 3 && i > 1){
	    list[i] = (index + 4);
	    i++;
	}
	else if ((index - 4) > -1 && i < 4 && i > 2){
	    list[i] = index - 4;
            i++;
	}
        else if ((index + 5) < 16 && i < 5 && i > 3 && ((index + 5) % 4) != 0){
            list[i] = index + 5;
            i++;
        }
        else if ((index - 5) > -1 && i < 6 && i > 4 && ((index % 4) !=0)){      
            list[i] = index - 5;
            i++;
        }
        else if ((index - 3) > 0 && i < 7 && i > 5 && ((index - 3) % 4) != 0){
            list[i] = index - 3;
            i++;
        }
        else if ((index + 3) < 16 && i < 8 && i > 6 && ((index % 4) != 0)){
            list[i] = index + 3;
            i++;
        }
        else{
            list[i] = -1;
            i++;
        }
    }
}

/***
Keeps track of letters currently being used to build word. 
***/
int letter_visited(int visited_list[15][2], int size, int index){
    int i = 0;
    int key = 0;
    while (i < size && key == 0){
        if(visited_list[i][0] == index){
            key = 1;
        }
        else{
            key = 0;
        }
        i++;
    }
    return key;
}

/***
Finds the index of the first letter of the word which the user has entered.
***/
void get_starting_index(int **starting_index_array, char *list, char letter){
    *starting_index_array = malloc(sizeof(int) * 16);
    int i = 0;
    int *index_array = *starting_index_array;
    while (i < 16){
        if(list[i] == letter){
            index_array[i] = i;
        
        }
        else{
            index_array[i] = -1;
        }
        i++;
    }
}

//Finds whether the word which the user entered has a valid path on the board.
char *path_finder(char *input, char *list, DNode **dictionary, DNode **submitted_d){
    int starting_index = -1;
    int visited_i = 0;
    int cd_i;
    int i;
    int j;
    int count = 1;
    int current_index = -1;
    int index_visited[15][2];
    int visited_size = 0;
    char *word = malloc(sizeof(strlen(input) + 1));
    char *input_upper = convert_upper(input);
    DNode *result;
    word[strlen(input)] = '\0';
    int *starting_indexes;
    int *current_directions;
    if (strlen(input) > 2){
    //Find multiple starting points if the first letter of the word
    //occurs more than once.
    get_starting_index(&starting_indexes, list, input_upper[0]);
         for (i = 0; i < 16; i++){
            starting_index = starting_indexes[i];
            count = 1;
            //-1 means that the index does not contain the first letter of the word.
            if (starting_index != -1){
                j = 1;
                word[0] = list[starting_index];
                //Add the letter has been visited
				index_visited[visited_i][0] = starting_index;
				index_visited[visited_i][1] = 0;
				visited_size++;
				visited_i++;
				//get directions on which direction to go
				current_index = starting_index;
				possible_directions(current_index, &current_directions);
				cd_i = 0;
	   
				while (visited_size > 0 && strcmp(input_upper, word) != 0){
					int visited = letter_visited(index_visited, visited_size, current_directions[cd_i]);
					//if path is invalid move onto the next direction
					if(current_directions[cd_i] == -1 && cd_i < 7){
						cd_i++;
					} 
					else if (visited && cd_i < 7){
						cd_i++;
					}
					else if (visited_size == 1 && cd_i > 7){
						visited_size = 0;
					}
					//if found add letter to visited, and increment 
					//to the next letter which needs to be found to build the word.
					else if (list[current_directions[cd_i]] == input_upper[j] && cd_i < 8){
						word[count] = list[current_directions[cd_i]];
						current_index = current_directions[cd_i];
						index_visited[visited_i][0] = current_index;
						index_visited[visited_i][1] = cd_i;
						visited_size++;
						visited_i++;
						free(current_directions);
						//get directions for next letter in word
						possible_directions(current_index, &current_directions);
						count++;
						j++;
						cd_i=0;;
					}
					else if (list[current_directions[cd_i]] != input_upper[j] && cd_i > 6){
					    //If the maximum depth is reached return back one letter and
					    //check remaining paths. Repeat until starting letter is reached.
						free(current_directions);
						cd_i = index_visited[visited_size - 1][1];
						index_visited[visited_size - 1][0] = -1;
						index_visited[visited_size - 1][1] = -1;
						visited_size--;
						visited_i--;
						current_index = index_visited[visited_size - 1][0]; 
						possible_directions(current_index, &current_directions);
						cd_i ++;
						count--;
						j--;
					}
					else if (list[current_directions[cd_i]] != input_upper[j] && cd_i < 7){
						cd_i++;
					}
				}   
	  
			}
        }	
    }
    return word;
    free(input_upper);
    free(starting_indexes);
    free(current_directions);
    
}
//Returns valid word for qu case
int qu_checker(char *input, char *list, DNode **dictionary, DNode **submitted_d){
    int j = 1;
    int i;
    DNode *result;
    char *q_input = malloc(strlen(input));
    q_input[0] = input[0];
    for(i=2; i < strlen(input); i++){
        q_input[j] = input[i];
        j++;
    }
    char *q_input_upper = convert_upper(q_input);
    char *input_upper = convert_upper(input);
    char *u_path_found = path_finder(input_upper, list, dictionary, submitted_d);
    char *path_found = path_finder(q_input_upper, list, dictionary, submitted_d);
    if (strcmp(input_upper, u_path_found) == 0 || strcmp(q_input_upper, path_found) == 0){
        char *input_lower;
        input_lower = convert_lower(input_upper);
        result = lookup(dictionary, BIG_HASH_SIZE, input_lower);
        if (result != NULL){
            char *result_word;
            result_word = convert_lower(result->key);
            if(strcmp(result_word, input_lower)==0 && lookup(submitted_d, SMALL_HASH_SIZE, input_lower) == NULL){
                insert(submitted_d, SMALL_HASH_SIZE, input_lower);
                return 0;
            }
            else{
                return 1;
            }  
        }
        else{
            return 1;
        }
    }
    else{
        return 1;
    }
    free(q_input);
    free(input_upper);
    free(q_input_upper);
    free(input_upper);
    free(u_path_found);
    free(path_found);
}

//Find path on board and checks whether it is a valid english word.
//If both conditions are true it returns 0 else 1
int word_checker(char *input, char *list, DNode **dictionary, DNode **submitted_d){
    char *word;
    DNode* result;
    char *input_upper = convert_upper(input);
    word = path_finder(input_upper, list, dictionary, submitted_d);
    char *word_lower;
    word_lower = convert_lower(word);
    result = lookup(dictionary, BIG_HASH_SIZE, word_lower);
    if (result != NULL){
        char *result_word;
        result_word = convert_lower(result->key);
        if(strlen(word) == strlen(input) && strcmp(word, input_upper)==0){
            if (strcmp(result_word, word_lower) == 0 && lookup(submitted_d, SMALL_HASH_SIZE, word_lower) == NULL){
                insert(submitted_d, SMALL_HASH_SIZE, word_lower);
                return 0;
            }
            else{
                return 1;
            }  
        }
        else{
            return 1;
        }
    }
    else{
        return 1;
    }
    free(input_upper);
    free(word);
    free(word_lower);
}

//Initiates game for main program
int start_game(char *board_letters, DNode **dictionary, char* userfile, Node **users){
     char name[20];
     char input[17];
     int score = 0;
	 int word_value;
	 static DNode* submitted_dict [SMALL_HASH_SIZE];
	 printf("Enter q to quit, n to start a new game, or\nEnter a word: ");
	 scanf("%s", input);
	 //waits for user to quit or start new game or enter another word
	 while (strcmp(input, "q") != 0 && strcmp(input, "n") != 0){
	     if (word_checker(input, board_letters, dictionary, submitted_dict) == 0){
	        display_board(board_letters);
	        word_value = score;
	        current_score(&score, input);
	        word_value = score - word_value;
	        printf("Valid word! %s\n", input);
	        printf("Word value: %d | Current Score: %d\n", word_value, score );
	        printf("Enter q to quit, n to start a new game, or\nEnter another word: ");
	    }
	    //For case with qu at the beginning of the word
	    else if ((input[0] == 'Q' || input[0] == 'q') && (input[1] == 'u' ||
	           input[1] == 'U') && qu_checker(input,
	            board_letters, dictionary, submitted_dict) == 0){
	        display_board(board_letters);
	        word_value = score;
	        current_score(&score, input);
	        word_value = score - word_value;
	        printf("Valid word! %s\n", input);
	        printf("Word value: %d | Current Score: %d\n", word_value, score );
	        printf("Enter another word or enter q to quit: ");    
	        
	    }
	    else if(word_checker(input, board_letters, dictionary, submitted_dict) == 1){
	        display_board(board_letters);
	        printf("Your word is either invalid or already used.\n");
	        printf("Enter another word or enter q to quit: ");
	    }
        scanf("%s", input);
	}       
	printf("Current Score: %d\n", score);
	return score;    
    free_dictionary(submitted_dict, SMALL_HASH_SIZE);
}
