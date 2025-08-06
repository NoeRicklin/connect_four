#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/neural_network_setup.h"

#define PI 3.141592

int hidden_layer_lengths[] = {INPUT_LAYER_LENGTH, 16};
float *layers[NUM_HIDDEN_LAYERS + 1];
float output_layer[OUTPUT_LAYER_LENGTH];

void neural_network_ram_setup() {
    for (int i = 0; i < NUM_HIDDEN_LAYERS + 1; i++) {
        layers[i] = (float *) malloc(hidden_layer_lengths[i] * sizeof(float));
    }
}

int get_num_parameters() {
	int num_parameters = 0;
	
	for (int layer = 0; layer < NUM_HIDDEN_LAYERS; layer++) {
		num_parameters += (hidden_layer_lengths[layer] + 1) * hidden_layer_lengths[layer + 1];
	}
	num_parameters += (hidden_layer_lengths[NUM_HIDDEN_LAYERS] + 1) * OUTPUT_LAYER_LENGTH;

	return num_parameters;
}

float **load_bots_parameters() {
	int num_parameters = get_num_parameters();
	char path_string[STRING_SIZE];

	float **bots_parameters = (float **)malloc(NUMBER_OF_BOTS * sizeof(float *));

	for (int bot_id = 0; bot_id < NUMBER_OF_BOTS; bot_id++) {
		sprintf(path_string, "bots_params/bot%i", bot_id);
		FILE *parameters_file = fopen(path_string, "r");

		bots_parameters[bot_id] = (float *)malloc(num_parameters * sizeof(float));
		fread(bots_parameters[bot_id], sizeof(float), num_parameters, parameters_file);

		if (bot_id == 0) {
			printf("bots_parameters[0]: %p\n", bots_parameters[0]);
		}

		fclose(parameters_file);
	}

	return bots_parameters;
}

void initialize_bot_files() {
	int num_parameters = get_num_parameters();
	char path_string[STRING_SIZE];

	float *randomized_parameters = (float *)malloc(num_parameters * sizeof(float));

	for (int bot_id = 0; bot_id < NUMBER_OF_BOTS; bot_id++) {
		sprintf(path_string, "bots_params/bot%i", bot_id);
		FILE *parameters_file = fopen(path_string, "w+");

		for (int parameter_index = 0; parameter_index < num_parameters; parameter_index++) {
			randomized_parameters[parameter_index] = random_float() - 0.5;
		}

		fwrite(randomized_parameters,
				sizeof(float), num_parameters,
				parameters_file);

		fclose(parameters_file);
	}
}

// returns number between 0 and 1 (not inclusive)
float random_float() {
	return (float)rand() / ((float)RAND_MAX + 1) + 0.000015259;
}

// Box-Muller
float standard_normal_distribution() {
	float u1 = random_float();
	float u2 = random_float();

	return sqrtf(-2*logf(u1)) * cosf(2 * PI * u2);
}
