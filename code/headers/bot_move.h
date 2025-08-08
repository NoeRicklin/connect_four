#ifndef BOT_MOVE
#define BOT_MOVE

int compare(const void *a, const void *b);
int bot_move(char *game_state, float *parameters, char player);

struct column_value{
	int column;
	float value;
};

#endif
