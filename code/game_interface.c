#include <stdio.h>
#include "headers/game_interface.h"
#include <stdlib.h>
#include <memory.h>

#include "headers/neural_network_setup.h"

char *game_state_p1; //game state from perspective of player 1
char *game_state_p2; //game state from perspective of player -1 (player -1 selfishly thinks he is player 1)
float bot_fitness[NUMBER_OF_BOTS];

unsigned char moves;

void initialise_game_states() {
    game_state_p1 = (char *) calloc(board_height * board_width, sizeof(char));
    game_state_p2 = (char *) calloc(board_height * board_width, sizeof(char));
}

void reset_game() {
    memset(game_state_p1, (char) 0, board_height * board_width * sizeof(char));
    memset(game_state_p2, (char) 0, board_height * board_width * sizeof(char));
    moves = 0;
}

int put_stone(int col, char player) {
    if (player != PLAYER_ONE && player != PLAYER_TWO) {
        return INVALID_PLAYER;
    }

    if (col < 0 || col >= board_width) {
        return INVALID_COLUMN;
    }

    int row = 0;
    while (game_state_p1[row * board_width + col] != EMPTY_TILE) {
        row++;
        if (row == board_height) {
            return FULL_COLUMN;
        }
    }

    game_state_p1[row * board_width + col] = player;
    game_state_p2[row * board_width + col] = -player;
    moves++;

    return PUT_SUCCESS;
}

int test_win(int col, char player) {
    int result = 0;

    int row = board_height - 1;
    while (game_state_p1[row * board_width + col] == EMPTY_TILE) { row--; }

    if (_test_hor_win(row, player)) {
        result |= WIN;
        result |= HORIZONTAL_WIN;
    }

    if (_test_vert_win(col, player)) {
        result |= WIN;
        result |= VERTICAL_WIN;
    }

    if (_test_diag_win(row, col, player)) {
        result |= WIN;
        result |= DIAGONAL_WIN;
    }

    if (!(result & WIN)) {
        if (moves == 42) {
            result |= DRAW;
        } else {
            result |= NOT_OVER;
        }
    }

    *(&(char) result + 1) = moves;

    return result;
}

int _test_hor_win(int row, char player) {
    int counter = 0;

    for (int stone_index = 0; stone_index < board_width; stone_index++) {
        if (game_state_p1[row * board_width + stone_index] == player) {
            counter++;

            if (counter == 4) {
                return TRUE;
            }
        } else {
            counter = 0;
        }
    }

    return FALSE;
}

int _test_vert_win(int col, char player) {
    int counter = 0;

    for (int stone_index = 0; stone_index < board_width; stone_index++) {
        if (game_state_p1[stone_index * board_width + col] == player) {
            counter++;

            if (counter == 4) { return TRUE; }
        } else { counter = 0; }
    }

    return FALSE;
}

int _test_diag_win(int row, int col, char player) {
    // Algorithm: Determine first stone on the relevant diagonal using a
    // lookup-table indexed by the module of the index of the new stone,
    // which all stones on the same diagonal share. Also, only do this
    // for diagonals containing at least four tiles.

    if (_test_neg_diag_win(row, col, player) || _test_pos_diag_win(row, col, player)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int _test_neg_diag_win(int row, int col, char player) {
    int new_stone_index = row * board_width + col;

    int diag_start_index_lookup[6] = {36, 37, 38, 21, 28, 35};
    int diag_length_lookup[6] = {6, 5, 4, 4, 5, 6};
    int diag_offset = -6;

    int counter = 0;

    // check if new stone is in diagonal with 4+ stones
    if (row + col > 2 && row + col < 9) {
        int lookup_index = new_stone_index % 6;

        // for each stone in this diagonal
        int current_stone_index = diag_start_index_lookup[lookup_index];
        for (int stone_index = 0;
             stone_index < diag_length_lookup[lookup_index];
             stone_index++) {
            if (game_state_p1[current_stone_index] == player) {
                counter++;

                if (counter == 4) { return TRUE; }
            } else { counter = 0; }

            current_stone_index += diag_offset;
        }
        return FALSE;
    } else { return FALSE; }
}

int _test_pos_diag_win(int row, int col, char player) {
    int new_stone_index = row * board_width + col;

    int diag_start_index_lookup[8] = {40, 41, 34, 27, 0, 0, 38, 39};
    int diag_length_lookup[8] = {6, 6, 5, 4, 0, 0, 4, 5};
    int diag_offset = -8;

    int counter = 0;

    // check if new stone is in diagonal with 4+ stones
    if (row - col < 3 || row - col > -4) {
        int lookup_index = new_stone_index % 8;

        // for each stone in this diagonal
        int current_stone_index = diag_start_index_lookup[lookup_index];
        for (int stone_index = 0;
             stone_index < diag_length_lookup[lookup_index];
             stone_index++) {
            if (game_state_p1[current_stone_index] == player) {
                counter++;

                if (counter == 4) { return TRUE; }
            } else { counter = 0; }

            current_stone_index += diag_offset;
        }
        return FALSE;
    } else { return FALSE; }
}

void print_game_state() {
    puts(" 1234567 ");
    puts("---------");

    for (int row = board_height - 1; row >= 0; row--) {
        printf("|");

        for (int col = 0; col < board_width; col++) {
            switch (game_state_p1[row * board_width + col]) {
                case PLAYER_ONE: {
                    printf("x");
                }
                break;
                case PLAYER_TWO: {
                    printf("o");
                }
                break;
                case EMPTY_TILE: {
                    printf(" ");
                }
                break;
                default: {
                    printf("?");
                }
                break;
            }
        }

        printf("|\n");
    }

    puts("---------");
}
