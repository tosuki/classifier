#ifndef NETWORK_H
#define NETWORK_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INITIAL_LAYERS_ALLOCATED_MEM 4

typedef struct neuron {
  double value;
  double *weights;
  int weights_len;
} t_neuron;

typedef struct neuron_layer {
  int nodes_len;
  t_neuron **neurons;
} t_neuron_layer;

typedef struct network {
  t_neuron_layer **layers;
  int layers_len;
  int layers_allocated_mem;
} t_network;

// Funções de Gerenciamento da Rede

/**
 * Aloca e inicializa uma nova estrutura de rede neural.
 * @param network Ponteiro para o ponteiro da rede a ser criada.
 * @return 1 em caso de sucesso, 0 em caso de falha de alocação.
 */
int create_network(t_network **network);

/**
 * Cria uma nova camada de neurônios e a insere na rede.
 * Se houver uma camada anterior, os pesos dos novos neurônios serão inicializados aleatoriamente.
 * @param network Ponteiro para a rede onde a camada será inserida.
 * @param layer Ponteiro para o ponteiro da camada a ser criada.
 * @param values Valores iniciais para os neurônios da camada.
 * @param values_len Quantidade de neurônios na camada.
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
int create_neuron_layer(t_network *network, t_neuron_layer **layer,
                        double *values, int values_len);

/**
 * Calcula os valores de saída de uma camada baseando-se nos valores da camada anterior.
 * Realiza a operação matricial: output = activation(weights * inputs).
 * @param layer Camada atual (que receberá os novos valores).
 * @param prev_layer Camada anterior (que fornece os inputs).
 * @return 0 em caso de sucesso.
 */
int calculate_layer_output(t_neuron_layer *layer, t_neuron_layer *prev_layer);

/**
 * Libera toda a memória alocada para a rede, incluindo camadas, neurônios e pesos.
 * @param network Ponteiro para a rede a ser destruída.
 */
void free_network(t_network *network);

/**
 * Exibe no console a estrutura atual da rede, seus neurônios, valores e pesos.
 * @param network Ponteiro para a rede a ser impressa.
 */
void print_network(t_network *network);

// Funções de Cálculo

/**
 * Aplica a função de ativação Sigmoid a um valor.
 * @param value O valor de entrada (z).
 * @return O valor ativado entre 0 e 1.
 */
double activation_function(double value);

#endif
