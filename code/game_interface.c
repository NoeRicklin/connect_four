#define board_height 6
#define board_width 7

#define SUCCESS 0
#define INVALID_COLUMN 1
#define FULL_COLUMN 2
#define INVALID_PLAYER 3

#define PLAYER_ONE 1
#define PLAYER_TWO -1
#define EMPTY_TILE 0

char game_state[board_height*board_width];

int put_stone(int col, int player) {
    if (player != PLAYER_ONE && player != PLAYER_TWO) {
        return INVALID_PLAYER;
    }

    if (col < 0 || col >= board_width) {
        return INVALID_COLUMN;
    }

    int row = 0;
    while (game_state[row*board_width + col] != EMPTY_TILE) {
        row++;
        if (row == board_height - 1) {
            return FULL_COLUMN;
        }
    }

    game_state[row*board_width + col] = player;

    return SUCCESS;
}