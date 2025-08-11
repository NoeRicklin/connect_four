#define _CRT_SECURE_NO_WARNINGS 1

#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"
#include "headers/bot_move.h"
#include <time.h>
#include <math.h>
#include "headers/generation_creation.h"
#include <string.h>

#define ASCII_DIGIT_OFFSET 48
#define NUM_GENERATIONS 10000

extern char *game_state_p1;
extern char *game_state_p2;

char current_player = 1;
int running = 1;
int result;
extern float bot_fitness[NUMBER_OF_BOTS];
float **bots_parameters;
char input_buffer[2];

int main() {
	while (input_buffer[0] != 'y' && input_buffer[0] != 'n') {
		fprintf(stderr, "Reinitialize parameters? (y/n): ");
		fgets(input_buffer, 2, stdin);
	}

    initialise_game_states();
    neural_network_ram_setup();

	if (input_buffer[0] == 'y') {
		initialize_bot_files();
	}

	char *log_path = "..\\misc\\logs.csv";
	FILE *log_file = fopen(log_path, "w");
	fclose(log_file);
	log_file = fopen(log_path, "w");
	char *log_header = "Generation-ID,Calculation time,Average number of moves,Average Fitness,Highest Fitness,Lowest Fitness,Vertical Wins,Horizontal Wins,Diagonal Wins,Draws\n";
	fwrite(log_header, sizeof(char), strlen(log_header), log_file);
	fclose(log_file);

    bots_parameters = load_bots_parameters();

    for (int generation = 1; generation <= NUM_GENERATIONS; generation++) {
        clock_t generation_start = clock();

		int generation_total_moves = 0;
		int vertical_wins = 0;
		int horizontal_wins = 0;
		int diagonal_wins = 0;
		int draws = 0;

        for (int bot1 = 0; bot1 < NUMBER_OF_BOTS; bot1++) {
            for (int bot2 = 0; bot2 < NUMBER_OF_BOTS; bot2++) {
                float *parameters_p1 = bots_parameters[bot1];
                float *parameters_p2 = bots_parameters[bot2];
                char *game_state;
                float *parameters;
                running = 1;
                while (running) {
                    switch (current_player) {
                        case PLAYER_ONE:
                            game_state = game_state_p1;
                            parameters = parameters_p1;
                            break;
                        case PLAYER_TWO:
                            game_state = game_state_p2;
                            parameters = parameters_p2;
                            break;
                        default:
                            exit(EXIT_FAILURE);
                    }

                    int column = bot_move(game_state, parameters, current_player);

                    result = test_win(column, current_player);

                    if (result & WIN) {
						vertical_wins += (result & VERTICAL_WIN) >> 3;
						horizontal_wins += (result & HORIZONTAL_WIN) >> 4;
						vertical_wins += (result & DIAGONAL_WIN) >> 5;

						int moves = result >> GAME_MOVE_OFFSET;
						generation_total_moves += moves;
                        running = 0;
                        switch (current_player) {
                            case PLAYER_ONE:
                                bot_fitness[bot1] += (float)50.0 + (float)moves;
                                bot_fitness[bot2] -= fmaxf((float)20.0 - (float)moves, (float)0);
                                break;
                            case PLAYER_TWO:
                                bot_fitness[bot2] += (float)55.0 + (float)moves;
                                bot_fitness[bot1] -= fmaxf((float)22.0 - (float)moves, (float)0);
                                break;
                            default:
                                break;
                        }
                        reset_game();
                    }
                    if (result & DRAW) {
						draws += 1;
						int moves = result >> GAME_MOVE_OFFSET;
						generation_total_moves += moves;
                        running = 0;
                        reset_game();
                    }

					current_player *= -1;
				}
			}
		}
		clock_t generation_end = clock();
		struct fitness_stats fs = next_generation();

		printf("Generation %5i took %4.2f seconds with an average of %5.2f moves per game\n",
				generation,
				(float)(generation_end - generation_start) / CLOCKS_PER_SEC,
				generation_total_moves / (float)(NUMBER_OF_BOTS * NUMBER_OF_BOTS));

		log_file = fopen(log_path, "a");
		char generation_data[100];
		sprintf(generation_data, "%i,%f,%f,%f,%f,%f,%i,%i,%i,%i\n",
				generation,
				(float)(generation_end - generation_start) / CLOCKS_PER_SEC,
				generation_total_moves / (float)(NUMBER_OF_BOTS * NUMBER_OF_BOTS),
				fs.avg_fitness,
				fs.max_fitness,
				fs.min_fitness,
				vertical_wins,
				horizontal_wins,
				diagonal_wins,
				draws);
		fwrite(generation_data, sizeof(char), strlen(generation_data), log_file);
		fclose(log_file);
	}
}
