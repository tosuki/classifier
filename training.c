#include "training.h"

#include <stdio.h>
#include <stdlib.h>

int create_training_example(t_training_example **example, int *expected_output,
                            int expected_output_len) {
  if (expected_output_len <= 0 || !expected_output)
    return 0;

  (*example) = malloc(sizeof(t_training_example));

  if (!(*example)) {
    perror("Failed to allocate training example");
    exit(EXIT_FAILURE);
  }

  (*example)->expected_output_len = expected_output_len;
  (*example)->expected_output_allocated_mem = expected_output_len;
  (*example)->expected_output_len_used = expected_output_len;
  (*example)->expected_output = malloc(sizeof(int) * expected_output_len);

  if (!(*example)->expected_output) {
    free(*example);
    *example = NULL;
    perror("Failed to allocate expected output array");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < expected_output_len; i++) {
    (*example)->expected_output[i] = expected_output[i];
  }

  return 1;
}

int feedforward_training_example(t_network *network,
                                 t_training_example *example) {
  if (!network || !example || network->layers_len == 0)
    return 0;

  if (network->layers[network->layers_len - 1]->nodes_len !=
      example->expected_output_len)
    return 0;

  if (network->layers_len == 1) {
    for (int i = 0; i < network->layers[0]->nodes_len; i++) {
      network->layers[0]->neurons[i]->value =
          example->expected_output[i]; // Input layer directly takes expected
                                       // output
    }
    return 1;
  }

  if (network->layers_len > 1) {
    for (int i = 0; i < network->layers_len - 1; i++) {
      if (calculate_layer_output(network->layers[i + 1], network->layers[i]) ==
          0) {
        return 0;
      }
    }
  }

  return 1;
}

int free_training_example(t_training_example *example) {
  if (!example)
    return 0;

  if (example->expected_output)
    free(example->expected_output);

  free(example);
  return 1;
}
