#include "headers/neural_network_setup.h"
#include <stdlib.h>
#include <search.h>
#include "headers/bot_move.h"
#include "headers/game_interface.h"

extern float *layers[];
extern float output_layer[];


int bot_move(char *game_state, float *parameters, int player) {
    for (int i = 0; i < INPUT_LAYER_LENGTH; i++) {
        layers[0][i] = game_state[i];
    }
    for (int layer_index = 0; layer_index < NUM_HIDDEN_LAYERS; layer_index++) {
        for (int next_layer_node_index = 0; next_layer_node_index < hidden_layer_lengths[layer_index + 1];
             next_layer_node_index++) {
            int next_layer_current_node_accumulator = 0; // TODO: Add Bias
            for (int current_layer_node_index = 0; current_layer_node_index < hidden_layer_lengths[layer_index];
                 current_layer_node_index++) {
                next_layer_current_node_accumulator += parameters[0] * layers[layer_index][current_layer_node_index];
                // TODO: Parameters Formating
            }
            if (next_layer_current_node_accumulator < 0) {
                next_layer_current_node_accumulator = 0;
            }
            layers[layer_index + 1][next_layer_node_index] = next_layer_current_node_accumulator;
        }
    }
    for (int output_layer_node_index = 0; output_layer_node_index < OUTPUT_LAYER_LENGTH; output_layer_node_index++) {
        int output_layer_current_node_accumulator = 0; // TODO: Add Bias
        for (int last_hidden_layer_node_index = 0;
             last_hidden_layer_node_index < hidden_layer_lengths[NUM_HIDDEN_LAYERS]; last_hidden_layer_node_index++) {
            output_layer_current_node_accumulator += parameters[0] * layers[NUM_HIDDEN_LAYERS][
                last_hidden_layer_node_index];
        }
        output_layer[output_layer_node_index] = output_layer_current_node_accumulator;
    }
    qsort(output_layer, OUTPUT_LAYER_LENGTH, sizeof(float), compare);

    for (int current_best_column = OUTPUT_LAYER_LENGTH; current_best_column >=0; current_best_column--) {
        if (put_stone(current_best_column, player) ==  PUT_SUCCESS) {
            return current_best_column;
        }
    }

    return -1;
}

int compare(float *a, float *b) {
    if (*a > *b) {
        return 4;
    } else if (*a < *b) {
        return -4;
    } else {
        return 0;
    }
}