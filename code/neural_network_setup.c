#include <stdio.h>
#define _CRT_RAND_S
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "headers/neural_network_setup.h"

#define PI (float)3.141592

int hidden_layer_lengths_initialiser[] = {INPUT_LAYER_LENGTH, 16};
int *hidden_layer_lengths;
float **layers;
float *output_layer;

void neural_network_ram_setup() {
    layers = (float **) malloc((NUM_HIDDEN_LAYERS + 1) * sizeof(float *));
    output_layer = (float *) malloc(OUTPUT_LAYER_LENGTH * sizeof(float));
    hidden_layer_lengths = (int *) malloc((NUM_HIDDEN_LAYERS + 1) * sizeof(int));

    for (int i = 0; i < NUM_HIDDEN_LAYERS + 1; i++) {
        hidden_layer_lengths[i] = hidden_layer_lengths_initialiser[i];
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
    FILE *parameters_file;

    float **bots_parameters = (float **) malloc(NUMBER_OF_BOTS * sizeof(float *));

    for (int bot_id = 0; bot_id < NUMBER_OF_BOTS; bot_id++) {
        sprintf_s(path_string, sizeof(char) * STRING_SIZE, "..\\code\\bots_params\\bot%i", bot_id);
        fopen_s(&parameters_file, path_string, "rb");

        bots_parameters[bot_id] = (float *) malloc(num_parameters * sizeof(float));
        fread(bots_parameters[bot_id], sizeof(float), num_parameters, parameters_file);
        fclose(parameters_file);
    }

    return bots_parameters;
}
void store_bots_parameters(float **bots_parameters) {
    int num_parameters = get_num_parameters();
    char path_string[STRING_SIZE];
    FILE *parameters_file;

    for (int bot_id = 0; bot_id < NUMBER_OF_BOTS; bot_id++) {
        sprintf_s(path_string, sizeof(char) * STRING_SIZE, "..\\code\\bots_params\\bot%i", bot_id);
        fopen_s(&parameters_file, path_string, "wb");

        fwrite(bots_parameters[bot_id], sizeof(float), num_parameters, parameters_file);
        fclose(parameters_file);
    }
}

void initialize_bot_files() {
    int num_parameters = get_num_parameters();
    char path_string[STRING_SIZE];
    FILE *parameters_file;

    float *randomized_parameters = (float *) malloc(num_parameters * sizeof(float));

    for (int bot_id = 0; bot_id < NUMBER_OF_BOTS; bot_id++) {
        sprintf_s(path_string, sizeof(float) * STRING_SIZE, "..\\code\\bots_params\\bot%i", bot_id);
        fopen_s(&parameters_file, path_string, "wb");

        for (int parameter_index = 0; parameter_index < num_parameters; parameter_index++) {
            randomized_parameters[parameter_index] = 100 * (random_float() - (float) 0.5);
        }

        fwrite(randomized_parameters,
               sizeof(float), num_parameters,
               parameters_file);

        fclose(parameters_file);
    }

	free(randomized_parameters);
}

// returns number between 0 and 1 (not inclusive)
float random_float() {
    unsigned int random_number;
    rand_s(&random_number);

    return (float) random_number / ((float) UINT_MAX + 1) + (float) 0.00000000000001;
}

// Box-Muller
float normal_distribution() {
    float u1 = random_float();
    float u2 = random_float();

    return (sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2)) * SIGMA;
}
