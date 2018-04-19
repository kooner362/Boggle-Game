main: boggle.c dictionary.c board.c generator.c scoreboard.c scoreboard.h dictionary.h board.h generator.h 
	gcc -o boggle boggle.c board.c dictionary.c generator.c scoreboard.c -I.