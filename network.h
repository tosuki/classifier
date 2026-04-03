#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define INITIAL_WEIGHTS_ALLOCATED_MEM;

typedef struct neuron {
    float value;
    float** weights;
    int weights_len;
    int weights_allocated_mem;
} t_neuron;

typedef struct neuron_layer {
    int nodes_len;
    t_neuron** neurons;
} t_neuron_layer;

typedef struct network {
    t_neuron_layer** layers;
    int layers_len;
    int layers_allocated_mem;
} t_network;

int create_network(t_network** network) {
    *network = (t_network*) malloc(sizeof(t_network));

    if (*network == NULL) {
        return 0;
    }

    return 1;
}

int create_neuron(t_neuron_layer* layer, t_neuron** neuron, double value) {
    *neuron = (t_neuron*) malloc(sizeof(t_neuron));

    if (*neuron == NULL) {
        return 0;
    }


    (*neuron)->value = value;
    (*neuron)->weights = malloc(sizeof(layer->nodes_len));
    (*neuron)->weights_len = layer->nodes_len;

    for (int i = 0; i < layer->nodes_len; i++) {
        (*neuron)->weights[i] = 1;
    }

    return 1;
}

int create_neuron_layer(t_neuron_layer** layer, double* values, int values_len) {
    *layer = (t_neuron_layer*) malloc(sizeof(t_neuron_layer));

    if (*layer == NULL) {
        return 0;
    }

    (*layer)->neurons = (t_neuron**) malloc(sizeof(t_neuron*)*values_len);
    (*layer)->nodes_len = values_len;

    for (int i = 0; i < values_len; i++) {
        t_neuron* neuron;

        if (!create_neuron(*layer, &neuron, values[i])) {
            printf("Failed to allocate memory to neuron %d of value %d\n", i, values[i]);
            exit(EXIT_FAILURE);
        }

        (*layer)->neurons[i] = neuron;
    }

    return 1;
}
/**
 * Sigmoid function
 * Range: 0 - 1
 */
double activation_function(double value) {
    return 1/(1+pow(2.718, -value));
}

#endif