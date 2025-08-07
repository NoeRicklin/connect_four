#include "headers/neural_network_setup.h"
#include <stdlib.h>
#include <search.h>
#include "headers/bot_move.h"
#include "headers/game_interface.h"
#include <stdio.h>

extern int *hidden_layer_lengths;
extern float **layers;
extern float *output_layer;


int bot_move(char *game_state, float *parameters, char player) {
    int parameter_index = 0;
    for (int i = 0; i < INPUT_LAYER_LENGTH; i++) {
        layers[0][i] = game_state[i];
    }

    for (int layer_index = 0;
         layer_index < NUM_HIDDEN_LAYERS;
         layer_index++) {
        for (int next_layer_node_index = 0;
             next_layer_node_index < hidden_layer_lengths[layer_index + 1];
             next_layer_node_index++) {
            float next_layer_current_node_accumulator = parameters[parameter_index++];

            for (int current_layer_node_index = 0;
                 current_layer_node_index < hidden_layer_lengths[layer_index];
                 current_layer_node_index++) {
                next_layer_current_node_accumulator += parameters[parameter_index++] * layers[layer_index][
                    current_layer_node_index];
            }
            if (next_layer_current_node_accumulator < 0) {
                next_layer_current_node_accumulator = 0;
            }
            layers[layer_index + 1][next_layer_node_index] = next_layer_current_node_accumulator;
        }
    }
    for (int output_layer_node_index = 0;
         output_layer_node_index < OUTPUT_LAYER_LENGTH;
         output_layer_node_index++) {
        float output_layer_current_node_accumulator = parameters[parameter_index++];
        for (int last_hidden_layer_node_index = 0;
             last_hidden_layer_node_index < hidden_layer_lengths[NUM_HIDDEN_LAYERS];
             last_hidden_layer_node_index++) {
            output_layer_current_node_accumulator += parameters[parameter_index++] * layers[NUM_HIDDEN_LAYERS][
                last_hidden_layer_node_index];
        }
        output_layer[output_layer_node_index] = output_layer_current_node_accumulator;
    }

    qsort(output_layer, OUTPUT_LAYER_LENGTH, sizeof(float), compare);

    for (int current_best_column = OUTPUT_LAYER_LENGTH; current_best_column >= 0; current_best_column--) {
        if (put_stone(current_best_column, player) == PUT_SUCCESS) {
            return current_best_column;
        }
    }

    return -1;
}

int compare(const void *a, const void *b) {
    float var1 = *(float *) a;
    float var2 = *(float *) b;

    if (var1 > var2) {
        return 4;
    } else if (var1 < var2) {
        return -4;
    } else {
        return 0;
    }
}
