#include "network.h"
#include "training.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    t_network *network;
    if (!create_network(&network)) {
        printf("Failed to create network\n");
        return 1;
    }

    double input_vals[] = {1.0, 0.5};
    t_neuron_layer *input_layer;
    create_neuron_layer(network, &input_layer, input_vals, 2);

    double hidden1_vals[] = {0.0, 0.0, 0.0};
    t_neuron_layer *hidden1_layer;
    create_neuron_layer(network, &hidden1_layer, hidden1_vals, 3);

    double hidden2_vals[] = {0.0, 0.0};
    t_neuron_layer *hidden2_layer;
    create_neuron_layer(network, &hidden2_layer, hidden2_vals, 2);

    double output_vals[] = {0.0};
    t_neuron_layer *output_layer;
    create_neuron_layer(network, &output_layer, output_vals, 1);

    t_training_example *example;
    int expected[] = {1};
    create_training_example(&example, expected, 1);

    printf("Calling feedforward_training_example...\n");
    int result = feedforward_training_example(network, example);
    printf("Result: %d\n", result);

    if (result == 1) {
        printf("Success!\n");
    } else {
        printf("Failure (as expected due to logic error)!\n");
    }

    return 0;
}
