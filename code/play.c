#include <stdio.h>
#include "game_interface.h"

#define ASCII_NUMBER_OFFSET 48

int current_player = 1;
char input[4];
int n = 4;
int finished = 0;
int result;
int stone_set_successfully;

int main() {
    while (!finished) {
        fgets(input, n, stdin);

        stone_set_successfully = put_stone(input[0] - ASCII_NUMBER_OFFSET, current_player);
        if (stone_set_successfully == PUT_SUCCESS) {
            result = test_win(input[0] - ASCII_NUMBER_OFFSET, current_player);
            print_game_state();
            if (WIN & result) {
                finished = 1;
                printf("Player %i won!\n", current_player);
            }
            current_player *= -1;
        } else {
            puts("Invalid input. Please repeat");
        }

    }
}
