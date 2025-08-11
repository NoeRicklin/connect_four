#ifndef NEURAL_NETWORK_SETUP
#define NEURAL_NETWORK_SETUP

//Neural network setup:
#define INPUT_LAYER_LENGTH 42
#define OUTPUT_LAYER_LENGTH 7
#define NUM_HIDDEN_LAYERS 1

#define NUMBER_OF_BOTS 100

#define STRING_SIZE 128

#define SIGMA 15

void neural_network_ram_setup();
float **load_bots_parameters();
int get_num_parameters();
size_t params_size();
void initialize_bot_files();
float random_float();
float normal_distribution();

#endif
