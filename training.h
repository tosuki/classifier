#ifndef TRAINING_H
#define TRAINING_H

#include "network.h"

int train_network(t_network *network, double **training_inputs,
                  double **training_outputs);
int error_rate(t_network *network, double **training_inputs,
               double **training_outputs);
int adjust_weights(t_network *network, double **training_inputs,
                   double **training_outputs);
int backpropagation(t_network *network, double **training_inputs,
                    double **training_outputs);

#endif
