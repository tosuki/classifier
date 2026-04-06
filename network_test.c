#include "network.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int g_failures = 0;

static void expect_true(int condition, const char *message) {
  if (condition) {
    printf("[OK]   %s\n", message);
  } else {
    printf("[FAIL] %s\n", message);
    g_failures++;
  }
}

static int almost_equal(double a, double b) { return fabs(a - b) < 1e-12; }

static void test_create_network(void) {
  t_network *network = NULL;

  int ok = create_network(NULL);
  expect_true(ok == 0, "create_network falha com ponteiro de saída NULL");

  ok = create_network(&network);
  expect_true(ok == 1, "create_network cria rede válida");
  expect_true(network != NULL, "create_network preenche ponteiro de saída");

  if (network) {
    expect_true(network->layers_len == 0, "rede inicia com layers_len = 0");
    expect_true(network->layers_allocated_mem == INITIAL_LAYERS_ALLOCATED_MEM,
                "rede inicia com memória inicial de camadas");
  }

  free_network(network);
}

static void test_create_neuron_layer(void) {
  t_network *network = NULL;
  t_neuron_layer *layer = NULL;
  double values[] = {1.0, 2.0};

  create_network(&network);

  int ok = create_neuron_layer(NULL, &layer, values, 2);
  expect_true(ok == 0, "create_neuron_layer falha com network NULL");

  ok = create_neuron_layer(network, NULL, values, 2);
  expect_true(ok == 0, "create_neuron_layer falha com layer NULL");

  ok = create_neuron_layer(network, &layer, NULL, 2);
  expect_true(ok == 0, "create_neuron_layer falha com values NULL");

  ok = create_neuron_layer(network, &layer, values, 0);
  expect_true(ok == 0, "create_neuron_layer falha com values_len inválido");

  ok = create_neuron_layer(network, &layer, values, 2);
  expect_true(ok == 1, "create_neuron_layer cria camada de entrada");
  expect_true(layer != NULL, "camada criada é não-NULL");
  expect_true(network->layers_len == 1, "camada foi inserida na rede");

  if (ok && layer) {
    expect_true(layer->nodes_len == 2, "camada possui número de nós correto");
    expect_true(layer->neurons[0]->weights_len == 0,
                "camada de entrada não possui pesos");
    expect_true(layer->neurons[0]->weights == NULL,
                "pesos de entrada são NULL");
  }

  t_neuron_layer *hidden = NULL;
  double hidden_values[] = {0.0, 0.0, 0.0};
  ok = create_neuron_layer(network, &hidden, hidden_values, 3);
  expect_true(ok == 1, "create_neuron_layer cria camada oculta");

  if (ok && hidden) {
    expect_true(hidden->neurons[0]->weights_len == 2,
                "camada oculta possui pesos para todos inputs");
    expect_true(hidden->neurons[0]->weights != NULL,
                "pesos da camada oculta foram alocados");
  }

  free_network(network);
}

static void test_activation_function(void) {
  expect_true(almost_equal(activation_function(0.0), 0.5),
              "sigmoid(0) == 0.5");
  expect_true(activation_function(10.0) > 0.99,
              "sigmoid(10) aproxima de 1");
  expect_true(activation_function(-10.0) < 0.01,
              "sigmoid(-10) aproxima de 0");
}

static void test_calculate_layer_output(void) {
  t_network *network = NULL;
  t_neuron_layer *input = NULL;
  t_neuron_layer *output = NULL;
  double input_values[] = {1.0, 2.0};
  double output_values[] = {0.0};

  create_network(&network);
  create_neuron_layer(network, &input, input_values, 2);
  create_neuron_layer(network, &output, output_values, 1);

  int ok = calculate_layer_output(NULL, input);
  expect_true(ok == 0, "calculate_layer_output falha com layer NULL");

  ok = calculate_layer_output(output, NULL);
  expect_true(ok == 0, "calculate_layer_output falha com prev_layer NULL");

  output->neurons[0]->weights[0] = 0.5;
  output->neurons[0]->weights[1] = -0.25;

  ok = calculate_layer_output(output, input);
  expect_true(ok == 1, "calculate_layer_output executa com entradas válidas");

  if (ok) {
    expect_true(almost_equal(output->neurons[0]->value, 0.5),
                "output calculado corresponde à sigmoid do produto matricial");
  }

  free_network(network);
}

int main(void) {
  srand(1234);

  test_create_network();
  test_create_neuron_layer();
  test_activation_function();
  test_calculate_layer_output();

  if (g_failures != 0) {
    printf("\nResultado: %d teste(s) falharam.\n", g_failures);
    return EXIT_FAILURE;
  }

  printf("\nResultado: todos os testes passaram.\n");
  return EXIT_SUCCESS;
}
