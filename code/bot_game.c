#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"

#define ASCII_DIGIT_OFFSET 48

#define BOT1 0
#define BOT2 1

extern char *game_state_p1;
extern char *game_state_p2;

int current_player = 1;
int running = 1;
int result;

int main() {
	print_game_state();

	neural_network_ram_setup();
	initialize_bot_files();
	float **bots_parameters = load_bots_parameters();
	printf("bots_parameters address: %p\n", bots_parameters);
	printf("first parameter of first bot: %f\n", bots_parameters[0][0]);

	float *parameters_p1 = bots_parameters[BOT1];
	float *parameters_p2 = bots_parameters[BOT2];

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

		char *game_state;
		float *parameters;

		switch(current_player) {
			case PLAYER_ONE:
				game_state = game_state_p1;
				parameters = parameters_p1;
				break;
			case PLAYER_TWO:
				game_state = game_state_p2;
				parameters = parameters_p2;
				break;
			default:
				game_state = NULL;
				parameters = NULL;
				break;
		}

		int column = bot_move(*game_state, parameters, current_player);

		result = test_win(column, current_player);
		print_game_state();

		if (result & WIN) {
			running = 0;
			printf("Player %i won!\n", current_player);
		}

		current_player *= -1;
    }
}
