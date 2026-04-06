#ifndef TRAINING_H
#define TRAINING_H

#include "network.h"

typedef struct training_example {
  int expected_output_len;
  int expected_output_allocated_mem;
  int expected_output_len_used;
  int *expected_output;
} t_training_example;

int create_training_example(t_training_example **example,
             const int *expected_output,
             int expected_output_len);

int feedforward_training_example(t_network *network,
               const t_training_example *example);

int get_error_for_training_example(t_network *network,
              const t_training_example *example,
              double *error);

int backpropagation_training_example(t_network *network,
                t_training_example *example);

int free_training_example(t_training_example *example);

#endif
