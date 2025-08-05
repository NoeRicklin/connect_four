#include "headers/neural_network_setup.h"

#include <stdlib.h>

float *layers[NUM_HIDDEN_LAYERS + 1];
float output_layer[OUTPUT_LAYER_LENGTH];

void bot_memory_setup() {
    for (int i = 0; i < NUM_HIDDEN_LAYERS + 1; i++) {
        layers[i] = (float *) malloc(hidden_layer_lengths[i] * sizeof(float));
    }
}

void bot_setup(int bot_id) {
}
