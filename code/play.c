#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"

#define ASCII_DIGIT_OFFSET 48
#define COLUMN_BUFFER_LEN 10
#define EXIT_CHAR 'q'

int current_player = 1;
char input[4];
int running = 1;
int result;

int main() {
	puts("Enter <q> to exit");	
	print_game_state();

    while (running) {
		char stone;
		switch (current_player) {
			case PLAYER_ONE:
				stone = 'x';
				break;
			case PLAYER_TWO:
				stone = 'o';
				break;
			default:
				exit(EXIT_FAILURE);
		}

		printf("You are Player %i (%c), enter column: ", current_player, stone);
        fgets(input, COLUMN_BUFFER_LEN, stdin);

		if (input[0] == EXIT_CHAR) { exit(EXIT_SUCCESS); }
		int column = input[0] - ASCII_DIGIT_OFFSET - 1;

        int put_stone_error = put_stone(column, current_player);
        if (put_stone_error == PUT_SUCCESS)
		{
            result = test_win(column, current_player);
            print_game_state();

            if (result & WIN) {
                running = 0;
                printf("Player %i won!\n", current_player);
            }

            current_player *= -1;
        }
		else
		{
            puts("Invalid input. Please repeat.");
			putc(7, stdout);
        }
    }
}