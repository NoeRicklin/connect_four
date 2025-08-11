#ifndef CONNECT_FOUR_GENERATION_CREATION
#define CONNECT_FOUR_GENERATION_CREATION

#define NUM_SURVIVORS 30

struct bot_fitness_struct {
    int bot_id;
    float fitness;
};

void next_generation();
void make_child(int parent_id, int child_id);
int compare_bots(const void *a, const void *b);

#endif