#include "network.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>

/**
 * Insere uma camada no array dinâmico da rede.
 * Realoca memória se o espaço atual for insuficiente.
 * @param network Ponteiro para a rede.
 * @param layer Ponteiro para a camada a ser inserida.
 * @return 1 em caso de sucesso, 0 em caso de erro na realocação.
 */
static int insert_layer_to_network(t_network *network, t_neuron_layer *layer) {
  if (network->layers_len >= network->layers_allocated_mem) {
    int new_size = network->layers_allocated_mem * 2;
    if (new_size == 0)
      new_size = INITIAL_LAYERS_ALLOCATED_MEM;

    t_neuron_layer **reallocated =
        realloc(network->layers, sizeof(t_neuron_layer *) * new_size);
    if (!reallocated)
      return 0;

    network->layers = reallocated;
    network->layers_allocated_mem = new_size;
  }

  network->layers[network->layers_len++] = layer;
  return 1;
}

/**
 * Cria a estrutura inicial da rede neural e aloca memória para o array de
 * camadas.
 * @param network Ponteiro para o ponteiro da rede a ser criada.
 * @return 1 em caso de sucesso, 0 em caso de falha de alocação.
 */
int create_network(t_network **network) {
  if (!network)
    return 0;

  *network = NULL;
  *network = (t_network *)malloc(sizeof(t_network));
  if (!*network)
    return 0;

  (*network)->layers_len = 0;
  (*network)->layers_allocated_mem = INITIAL_LAYERS_ALLOCATED_MEM;
  (*network)->layers = (t_neuron_layer **)malloc(
      sizeof(t_neuron_layer *) * (*network)->layers_allocated_mem);

  if (!(*network)->layers) {
    free(*network);
    *network = NULL;
    return 0;
  }

  return 1;
}

/**
 * Auxiliar para depuração: imprime uma matriz GSL no console.
 * @param matrix A matriz GSL a ser impressa.
 * @return 0 após a impressão.
 */
int print_matrix(gsl_matrix *matrix) {
  for (size_t i = 0; i < matrix->size1; i++) {
    for (size_t j = 0; j < matrix->size2; j++) {
      printf("%.3f ", gsl_matrix_get(matrix, i, j));
    }
    printf("\n");
  }
  return 1;
}

/**
 * O calculo é matrix de pesos * matrix coluna de inputs.
 * Resultando no novo valor (value) para cada neurônio da camada atual.
 */
int calculate_layer_output(t_neuron_layer *layer,
                           const t_neuron_layer *prev_layer) {
  if (!layer || !prev_layer || layer->nodes_len <= 0 || prev_layer->nodes_len <= 0)
    return 0;

  // Matriz de pesos: [Atual x Anterior]
  gsl_matrix *weights =
      gsl_matrix_alloc(layer->nodes_len, prev_layer->nodes_len);

  if (!weights) {
    perror("Failed to allocate weights matrix");
    return 0;
  }

  // Matriz de inputs: [Anterior x 1]
  gsl_matrix *inputs = gsl_matrix_alloc(prev_layer->nodes_len, 1);

  if (!inputs) {
    gsl_matrix_free(weights);
    perror("Failed to allocate inputs matrix");
    return 0;
  }

  // Matriz de outputs (z): [Atual x 1]
  gsl_matrix *outputs = gsl_matrix_alloc(layer->nodes_len, 1);

  if (!outputs) {
    gsl_matrix_free(weights);
    gsl_matrix_free(inputs);
    perror("Failed to allocate outputs matrix");
    return 0;
  }

  // Preenche a matriz de pesos
  for (int i = 0; i < layer->nodes_len; i++) {
    for (int j = 0; j < prev_layer->nodes_len; j++) {
      gsl_matrix_set(weights, i, j, layer->neurons[i]->weights[j]);
    }
  }

  // Preenche a matriz de inputs com os valores da camada anterior
  for (int j = 0; j < prev_layer->nodes_len; j++) {
    gsl_matrix_set(inputs, j, 0, prev_layer->neurons[j]->value);
  }

  // Multiplicação: outputs = 1.0 * (weights * inputs) + 0.0 * outputs
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, weights, inputs, 0.0,
                 outputs);

  // Aplica a função de ativação e salva o valor nos neurônios da camada atual
  for (int i = 0; i < layer->nodes_len; i++) {
    double z = gsl_matrix_get(outputs, i, 0);
    layer->neurons[i]->value = activation_function(z);
  }

  gsl_matrix_free(weights);
  gsl_matrix_free(inputs);
  gsl_matrix_free(outputs);

  return 1;
}

/**
 * Cria uma nova camada de neurônios, define seus valores iniciais e gera pesos
 * aleatórios baseados na camada anterior da rede.
 * @param network Ponteiro para a rede.
 * @param layer Ponteiro para o ponteiro da camada a ser criada.
 * @param values Valores iniciais dos neurônios.
 * @param values_len Quantidade de neurônios.
 * @return 1 em caso de sucesso, 0 em caso de erro.
 */
int create_neuron_layer(t_network *network, t_neuron_layer **layer,
                        const double *values, int values_len) {
  if (!network || !layer || !values || values_len <= 0)
    return 0;

  *layer = (t_neuron_layer *)malloc(sizeof(t_neuron_layer));
  if (!*layer)
    return 0;

  (*layer)->nodes_len = values_len;
  (*layer)->neurons = (t_neuron **)malloc(sizeof(t_neuron *) * values_len);

  if (!(*layer)->neurons) {
    free(*layer);
    return 0;
  }

  int weights_count = 0;
  if (network->layers_len > 0) {
    weights_count = network->layers[network->layers_len - 1]->nodes_len;
  }

  for (int i = 0; i < values_len; i++) {
    t_neuron *neuron = (t_neuron *)malloc(sizeof(t_neuron));
    if (!neuron)
      return 0;

    neuron->value = values[i];
    neuron->weights_len = weights_count;
    neuron->weights = NULL;

    if (weights_count > 0) {
      neuron->weights = (double *)malloc(sizeof(double) * weights_count);

      if (neuron->weights == NULL) {
        perror("Failed to allocate weights for neuron");
        return 0;
      }

      for (int w = 0; w < weights_count; w++) {
        neuron->weights[w] = ((double)rand() / (double)RAND_MAX) - 0.5;
      }
    }

    (*layer)->neurons[i] = neuron;
  }

  if (!insert_layer_to_network(network, *layer)) {
    return 0;
  }

  return 1;
}

/**
 * Função de ativação Sigmoid: transforma qualquer valor real para o intervalo
 * (0, 1).
 * @param value Valor de entrada.
 * @return Valor transformado.
 */
double activation_function(double value) { return 1.0 / (1.0 + exp(-value)); }

/**
 * Libera toda a memória dinâmica da rede: neurônios, pesos, camadas e a própria
 * rede.
 * @param network Ponteiro para a rede a ser destruída.
 */
void free_network(t_network *network) {
  if (!network)
    return;

  for (int l = 0; l < network->layers_len; l++) {
    t_neuron_layer *layer = network->layers[l];
    for (int n = 0; n < layer->nodes_len; n++) {
      t_neuron *neuron = layer->neurons[n];
      if (neuron->weights)
        free(neuron->weights);
      free(neuron);
    }
    free(layer->neurons);
    free(layer);
  }

  free(network->layers);
  free(network);
}

/**
 * Exibe a arquitetura da rede no terminal de forma legível.
 * @param network Ponteiro para a rede a ser exibida.
 */
void print_network(const t_network *network) {
  if (!network)
    return;

  printf("\n--- Neural Network Structure ---\n");
  for (int l = 0; l < network->layers_len; l++) {
    t_neuron_layer *layer = network->layers[l];
    char *type = (l == 0)                         ? "Input"
                 : (l == network->layers_len - 1) ? "Output"
                                                  : "Hidden";

    printf("\n[Layer %d] (%s)\n", l, type);
    for (int n = 0; n < layer->nodes_len; n++) {
      t_neuron *neuron = layer->neurons[n];
      printf("  \u25CB Neuron %d: Value: %.3f\n", n, neuron->value);

      if (neuron->weights_len > 0) {
        printf("    \u2514\u2500 Weights: [ ");
        for (int w = 0; w < neuron->weights_len; w++) {
          printf("%.3f%s", neuron->weights[w],
                 (w == neuron->weights_len - 1) ? "" : ", ");
        }
        printf(" ]\n");
      }
    }
  }
  printf("\n--------------------------------\n\n");
}
