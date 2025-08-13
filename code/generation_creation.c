#include "headers/generation_creation.h"
#include "headers/game_interface.h"
#include "headers/neural_network_setup.h"
#include <stdlib.h>
#include <memory.h>

extern float bot_fitness[NUMBER_OF_BOTS];
extern float **bots_parameters;

struct bot_fitness_struct bot_fitnesses[NUMBER_OF_BOTS];
int prev_bot_fitnesses[NUM_SURVIVORS];

struct fitness_stats next_generation() {
	struct fitness_stats fs = {0};

    for (int i = 0; i < NUMBER_OF_BOTS; i++) {
        bot_fitnesses[i].bot_id = i;
        bot_fitnesses[i].fitness = bot_fitness[i];
		fs.avg_fitness += bot_fitness[i];
    }
	fs.avg_fitness /= NUMBER_OF_BOTS;

    qsort(bot_fitnesses, NUMBER_OF_BOTS, sizeof(struct bot_fitness_struct), compare_bots);
    int parent_index = 0;
    for (int child_index = NUM_SURVIVORS; child_index < NUMBER_OF_BOTS; child_index++) {
        make_child(bot_fitnesses[parent_index++ % NUM_SURVIVORS].bot_id, bot_fitnesses[child_index].bot_id);
    }
	store_bots_parameters(bots_parameters);

	fs.max_fitness = bot_fitnesses[0].fitness;
	fs.min_fitness = bot_fitnesses[NUMBER_OF_BOTS - 1].fitness;

	fs.survivor_changes = NUM_SURVIVORS;
	for (int i = 0; i < NUM_SURVIVORS; i++) {
		for (int j = 0; j < NUM_SURVIVORS; j++) {
			if (bot_fitnesses[i].bot_id == prev_bot_fitnesses[j]) {
				fs.survivor_changes--;
			}
		}
	}

	for (int i = 0; i < NUM_SURVIVORS; i++) {
		prev_bot_fitnesses[i] = bot_fitnesses[i].bot_id;
	}

	memset(bot_fitness, 0, NUMBER_OF_BOTS * sizeof(float));
	return fs;
}

void make_child(int parent_id, int child_id) {
    for (int i = 0; i < get_num_parameters(); i++) {
        bots_parameters[child_id][i] = bots_parameters[parent_id][i] + normal_distribution();
    }
}

int compare_bots(const void *a, const void *b) {
    struct bot_fitness_struct var1 = *(struct bot_fitness_struct *) a;
    struct bot_fitness_struct var2 = *(struct bot_fitness_struct *) b;

    if (var1.fitness > var2.fitness) {
        return -4;
    } else if (var1.fitness < var2.fitness) {
        return 4;
    } else {
        return 0;
    }
}
