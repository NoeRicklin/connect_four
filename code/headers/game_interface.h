#ifndef GAME_INTERFACE
#define GAME_INTERFACE

#define FALSE		0
#define TRUE		1

// Board constants
#define board_height	6
#define board_width	7

// Put Errors
#define PUT_SUCCESS	0
#define INVALID_COLUMN	1
#define FULL_COLUMN	2
#define INVALID_PLAYER	3

// Win constants (Bit-activated)
#define NOT_OVER	0b00000001
#define WIN 		0b00000010
#define DRAW		0b00000100

#define VERTICAL_WIN	0b00001000
#define HORIZONTAL_WIN	0b00010000
#define DIAGONAL_WIN	0b00100000

#define GAME_MOVE_OFFSET 8

// Tile names
#define PLAYER_ONE	1
#define PLAYER_TWO	-1
#define EMPTY_TILE	0

void initialise_game_states();

int put_stone(int col, char player);
void print_game_state();
int test_win(int col, char player);

int _test_hor_win(int row, char player);
int _test_vert_win(int col, char player);
int _test_diag_win(int row, int col, char player);
int _test_neg_diag_win(int row, int col, char player);
int _test_pos_diag_win(int row, int col, char player);

#endif