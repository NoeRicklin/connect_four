#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"
#include "headers/bot_move.h"

#define ASCII_DIGIT_OFFSET 48

#define BOT1 0
#define BOT2 1

extern char *game_state_p1;
extern char *game_state_p2;

char current_player = 1;
int running = 1;
int result;

int main() {
    initialise_game_states();
    print_game_state();

    neural_network_ram_setup();
    initialize_bot_files();
    float **bots_parameters = load_bots_parameters();
    float *parameters_p1 = bots_parameters[BOT1];
    float *parameters_p2 = bots_parameters[BOT2];

    char stone;
    char *game_state;
    float *parameters;
    int column;

    while (running) {
        switch (current_player) {
            case PLAYER_ONE:
                stone = 'x';
                game_state = game_state_p1;
                parameters = parameters_p1;
                break;
            case PLAYER_TWO:
                stone = 'o';
                game_state = game_state_p2;
                parameters = parameters_p2;
                break;
            default:
                exit(EXIT_FAILURE);
        }

        column = bot_move(game_state, parameters, current_player);

        result = test_win(column, current_player);
        print_game_state();

        if (result & WIN) {
            running = 0;
            printf("Player %i won!\n", current_player);
            printf("Result: %x\n", result);
        }

        current_player *= -1;
    }
}
