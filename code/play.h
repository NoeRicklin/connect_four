#include <stdlib.h>
#include <stdio.h>
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"
#include "headers/bot_move.h"
#include "headers/generation_creation.h"


#define ASCII_DIGIT_OFFSET 48
#define COLUMN_BUFFER_LEN 10
#define EXIT_CHAR 'q'

int current_player = 1;
int bot_player = -1;
int bot_opponent = 0;
char input[4];
int running = 1;
int result;

extern char *game_state_p1;
extern char *game_state_p2;

float **bots_parameters;

int main() {
    while (input[0] != 'y' && input[0] != 'n') {
        printf("Do you want to play against a bot? (y/n) ");
        fgets(input, 2, stdin);
    }

    if (input[0] == 'y') {
        while (input[0] < '0' || input[0] > '9') {
            printf("Which bot? (enter number between 0 and 9): ");
            fgets(input, 2, stdin);
        }
        bot_opponent = input[0];
        while (input[0] != 'y' && input[0] != 'n') {
            printf("Do you want to play against a bot? (y/n) ");
            fgets(input, 2, stdin);
        }
        if (input[0] == 'n') {
            bot_player = 1;
        }

        initialise_game_states();
        neural_network_ram_setup();
        bots_parameters = load_bots_parameters();
        char *game_state;
        if (bot_player == 1) {
            game_state = game_state_p1;
        }else {
            game_state = game_state_p2;
        }
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

            if (current_player != bot_player) {
                printf("You are Player %i (%c), enter column: ", current_player, stone);
                fgets(input, COLUMN_BUFFER_LEN, stdin);

                if (input[0] == EXIT_CHAR) { exit(EXIT_SUCCESS); }
                int column = input[0] - ASCII_DIGIT_OFFSET - 1;

                int put_stone_error = put_stone(column, current_player);
                if (put_stone_error == PUT_SUCCESS) {
                    result = test_win(column, current_player);
                    print_game_state();

                    if (result & WIN) {
                        running = 0;
                        printf("Congratulations, you won!\n");
                    }

                    current_player *= -1;
                } else {
                    puts("Invalid input. Please repeat.");
                    putc(7, stdout);
                }
            }else {
                int column = bot_move(game_state, bots_parameters[bot_opponent], current_player);
                result = test_win(column, current_player);
                print_game_state();
                if (result & WIN) {
                    running = 0;
                    printf("Shame on you, you lost against a bot. Sounds like a skill issue to me. Because of you, humanity will lose the fight against AI.\n");
                }

                current_player *= -1;
            }


        }

    }else if (input[0] == 'n') {
        puts("Enter <q> to exit");
        initialise_game_states();
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
            if (put_stone_error == PUT_SUCCESS) {
                result = test_win(column, current_player);
                print_game_state();

                if (result & WIN) {
                    running = 0;
                    printf("Player %i won!\n", current_player);
                }

                current_player *= -1;
            } else {
                puts("Invalid input. Please repeat.");
                putc(7, stdout);
            }
        }
    }
}
