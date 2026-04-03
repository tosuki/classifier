#include "network.h"
#include <stdio.h>
#include <time.h>

int main() {
  srand(time(NULL));

  t_network *network;

  if (!create_network(&network)) {
    fprintf(stderr, "Failed to create network\n");
    return EXIT_FAILURE;
  }

  double input_values[] = {1.0, 0.5};
  t_neuron_layer *input_layer;
  if (!create_neuron_layer(network, &input_layer, input_values, 2)) {
    fprintf(stderr, "Failed to create input layer\n");
    free_network(network);
    return EXIT_FAILURE;
  }

  double hidden_values[] = {0.0, 0.0, 0.0};
  t_neuron_layer *hidden_layer;
  if (!create_neuron_layer(network, &hidden_layer, hidden_values, 3)) {
    fprintf(stderr, "Failed to create hidden layer\n");
    free_network(network);
    return EXIT_FAILURE;
  }

  printf("--- Antes do Cálculo ---\n");
  print_network(network);

  printf("\nCalculando output da camada oculta...\n");
  calculate_layer_output(hidden_layer, input_layer);

  printf("\n--- Depois do Cálculo ---\n");
  print_network(network);

  free_network(network);
  printf("Memória liberada com sucesso.\n");

  return EXIT_SUCCESS;
}
