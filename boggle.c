#include "generator.h"
#include "dictionary.h"
#include "scoreboard.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define FILENAME "EnglishWords.txt"
#define USERFILE "Users"

/***
Game boggle has two modes, user and tester. To enter tester a text file needs to
be provided. Boggle randomly displays a board, checks user input and keeps track
of score of users.
***/
int main(int argc, char **argv){
    int i;
    int j;
    int valid;
    int *score = malloc(sizeof(int));
	FILE *input_FP;
	char line [MAX_LINE];
	char * file_name;
	DNode* result;
	static DNode* dictionary [BIG_HASH_SIZE];
	static DNode* submitted_dict [SMALL_HASH_SIZE];
	
	//opens EnglishWords.txt
    file_name = FILENAME;
	if(!(input_FP = fopen ( file_name , "r" )))    {
        fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", file_name);
        return 1;
    }

	//trim new line characters and insrts trimmed words to dictionary
	while( fgets (line, MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  
		insert (dictionary, BIG_HASH_SIZE, line);
	}	
	fclose (input_FP);
	
	//If game is executed with 2 arguments, enters testing mode using provided test.txt file
	if (argc == 2){
	    char letters[17];
	    char buffer[100];
	    char words_buffer[100];
	    FILE *test_input;
	    char *token;
	    char *words[100];
	    char *invalid_words[100];
		
	    //read test file
	    if (!(test_input = fopen(argv[1], "r")))    {
            fprintf(stderr,"Could not open file \"%s\" for reading test words!\n", argv[1]);
        return 1;
        }
		
	    //Separates board letters from input words.
	    while (fgets(buffer, 100, test_input)!=NULL){
	        buffer[strcspn(buffer, "\r\n")] = '\0';
	        if (strlen(buffer) == 16){
	            strcpy(letters, buffer);
	        }
	        else{
	            strcpy(words_buffer, buffer);
	        }  
	    }
	    fclose(test_input);
		
	    char *upper_letters = convert_upper(letters);
	    i = 1;
	    //Separate all words by delimiter
	    token = strtok(words_buffer, ",");
	    words[0] = token;
	    while (token != NULL){
	        token = strtok(NULL, ",");
	        words[i] = token;
	        i++;
	    }
		
	    int size = i - 1;
	    i = 0;
	    j = 0;
	    while (i < size){
	    //Check for qu case or if word is valid increment score and check next word
	        if ((words[i][0] == 'q' || words[i][0] == 'Q') && 
	                (words[i][1] == 'u' || words[i][1] == 'U')){
	            valid = qu_checker(words[i], upper_letters, dictionary, submitted_dict);
	            if (valid == 0){
	                current_score(score, words[i]);
	                i++;
	            }
	            else if(valid == 1){
	                invalid_words[j] = words[i];
	                i++;
	                j++;
	            }
	        }
	        else{
	            valid = word_checker(words[i], upper_letters, dictionary, submitted_dict);
	            if (valid == 0){
	                current_score(score, words[i]);
	                i++;
	            }
	            else if(valid == 1){
	                invalid_words[j] = words[i];
	                i++;
	                j++;
	            }
	        }
	    }
	    int invalid_size = j - 1;
        char output[100];
 	    i = 1;
		
		
 	    //write output to text file.
	    FILE *output_result;
 	    output_result = fopen("result.txt", "w");
		
 	    if (invalid_size > 0){
	        strcpy(output, invalid_words[0]);
	        strcat(output, ",");
	        while (i < invalid_size){
	             strcat(output, invalid_words[i]);
	             strcat(output, ",");
	            i++;
	        }
	        strcat(output, invalid_words[i]);
	        fprintf(output_result, "%s\n", output);
	        fprintf(output_result, "%d", *score);
	        fclose(output_result);
 	    }
 	    else if (invalid_size == 0){
 	        strcpy(output, invalid_words[0]);
 	        fprintf(output_result, "%s\n", output);
	        fprintf(output_result, "%d", *score);
	        fclose(output_result);
 	        
 	    }
	    else{
	        fprintf(output_result, "\n");
	        fprintf(output_result, "%d", *score);
	        fclose(output_result);  
	    }
        free(score);
	}
	
	//User mode, where user will enter words manually
	else if (argc == 1){
	    char *board_letters;
	    char input[17];
	    char name[20];
	    int q_count = 0;
	    Node *users;
		
	    //create and randomize board
	    board_generator(&board_letters);
	    display_board(board_letters);
	    create_list(&users, USERFILE);
	    display_scores(&users);
		
	    while(q_count != 2){
	        //user needs to enter q twice to quit
	        display_board(board_letters);
	        *score = start_game(board_letters, dictionary, USERFILE, &users);
	        q_count++;
	        printf("Please enter your name: ");
	        scanf("%s", name);
	        if (existing_user(&users, name) != 1){
	            update_user(&users, name, *score);
	        }
	        else{
	            add_user(&users, name, *score, 1, *score);
	        }
	        system("clear");
	        display_scores(&users);
	        printf("Enter n for next game or q to quit: ");
	        scanf("%s", input);
	        
	        if (strcmp(input, "q") == 0){
	            
	            q_count++;
	        }
	        else if (strcmp(input, "n") == 0){
	            q_count = 0;
	        }
	        free(board_letters);
	        board_generator(&board_letters);
	    }
		
		//end of game, frees memories and saves user score
        system("clear");
        printf("Thanks for playing!\n");
        display_scores(&users);
        output_list(&users, USERFILE);
        free(board_letters);
        free(score);
        }
    
    free_dictionary(submitted_dict,SMALL_HASH_SIZE);
    free_dictionary(dictionary, BIG_HASH_SIZE); 
	return 0;
}
