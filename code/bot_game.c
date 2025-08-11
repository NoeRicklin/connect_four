#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"
#include "headers/bot_move.h"
#include <time.h>

#define ASCII_DIGIT_OFFSET 48

#define BOT1 0
#define BOT2 1

extern char *game_state_p1;
extern char *game_state_p2;

char current_player = 1;
int running = 1;
int result;

int main() {
    clock_t begin = clock();
    initialise_game_states();
    neural_network_ram_setup();
    initialize_bot_files();
    float **bots_parameters = load_bots_parameters();
    int game_id = 0;
    for (int bot1 = 0; bot1 < NUMBER_OF_BOTS; bot1++) {
        for (int bot2 = 0; bot2 < NUMBER_OF_BOTS; bot2++) {
            float *parameters_p1 = bots_parameters[bot1];
            float *parameters_p2 = bots_parameters[bot2];
            //printf("game: %i\n", game_id++);
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
                    running = 0;
                    //print_game_state();
                    //printf("Player %i won!\n", current_player);
                    //printf("Result: %x\n", result);
                    reset_game();
                }
                if (result & DRAW) {
                    running = 0;
                    //print_game_state();
                    //printf("It's a draw!");
                    //printf("Result: %x\n", result);
                    reset_game();
                }

                current_player *= -1;
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %f\n", (float)(end - begin) / CLOCKS_PER_SEC);
}
