#include "training.h"

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

static void test_create_training_example(void) {
  t_training_example *example = NULL;
  int values[] = {1, 0, 1};

  int ok = create_training_example(NULL, values, 3);
  expect_true(ok == 0, "create_training_example falha com saída NULL");

  ok = create_training_example(&example, NULL, 3);
  expect_true(ok == 0, "create_training_example falha com expected_output NULL");

  ok = create_training_example(&example, values, 0);
  expect_true(ok == 0, "create_training_example falha com len inválido");

  ok = create_training_example(&example, values, 3);
  expect_true(ok == 1, "create_training_example cria estrutura válida");

  if (ok && example) {
    expect_true(example->expected_output_len == 3,
                "expected_output_len definido corretamente");
    expect_true(example->expected_output[0] == 1 && example->expected_output[1] == 0 &&
                    example->expected_output[2] == 1,
                "valores esperados foram copiados");
  }

  free_training_example(example);
}

static void test_feedforward_training_example(void) {
  int expected[] = {1, 0};
  t_training_example *example = NULL;
  create_training_example(&example, expected, 2);

  int ok = feedforward_training_example(NULL, example);
  expect_true(ok == 0, "feedforward falha com network NULL");

  t_network *network = NULL;
  create_network(&network);

  ok = feedforward_training_example(network, NULL);
  expect_true(ok == 0, "feedforward falha com example NULL");

  ok = feedforward_training_example(network, example);
  expect_true(ok == 0, "feedforward falha com rede sem camadas");

  double input_vals[] = {0.0, 0.0};
  t_neuron_layer *input = NULL;
  create_neuron_layer(network, &input, input_vals, 2);

  ok = feedforward_training_example(network, example);
  expect_true(ok == 1, "feedforward com 1 camada aplica expected na entrada");
  if (ok) {
    expect_true(almost_equal(input->neurons[0]->value, 1.0),
                "valor esperado copiado para neurônio 0");
    expect_true(almost_equal(input->neurons[1]->value, 0.0),
                "valor esperado copiado para neurônio 1");
  }

  double hidden_vals[] = {0.0};
  t_neuron_layer *hidden = NULL;
  create_neuron_layer(network, &hidden, hidden_vals, 1);
  hidden->neurons[0]->weights[0] = 1.0;
  hidden->neurons[0]->weights[1] = -1.0;

  ok = feedforward_training_example(network, example);
  expect_true(ok == 0, "feedforward falha quando output_len difere de expected");

  int expected_one[] = {1};
  t_training_example *example_one = NULL;
  create_training_example(&example_one, expected_one, 1);

  input->neurons[0]->value = 1.0;
  input->neurons[1]->value = 1.0;
  ok = feedforward_training_example(network, example_one);
  expect_true(ok == 1, "feedforward com múltiplas camadas executa");
  if (ok) {
    expect_true(almost_equal(hidden->neurons[0]->value, 0.5),
                "feedforward calcula camada seguinte corretamente");
  }

  free_training_example(example_one);
  free_training_example(example);
  free_network(network);
}

static void test_get_error_for_training_example(void) {
  t_network *network = NULL;
  t_training_example *example = NULL;
  double error = -1.0;
  int expected[] = {1};

  int ok = get_error_for_training_example(NULL, NULL, &error);
  expect_true(ok == 0, "get_error falha com network NULL");

  create_network(&network);
  ok = get_error_for_training_example(network, NULL, &error);
  expect_true(ok == 0, "get_error falha sem camadas/example");

  double output_vals[] = {0.0};
  t_neuron_layer *output = NULL;
  create_neuron_layer(network, &output, output_vals, 1);
  create_training_example(&example, expected, 1);

  ok = get_error_for_training_example(network, example, NULL);
  expect_true(ok == 0, "get_error falha com ponteiro de erro NULL");

  ok = get_error_for_training_example(network, example, &error);
  expect_true(ok == 1, "get_error retorna sucesso para entradas válidas");
  if (ok) {
    expect_true(almost_equal(error, 0.0), "get_error zera erro na implementação atual");
  }

  int expected_bad[] = {1, 0};
  t_training_example *bad = NULL;
  create_training_example(&bad, expected_bad, 2);
  error = -1.0;
  ok = get_error_for_training_example(network, bad, &error);
  expect_true(ok == 0, "get_error falha quando tamanho esperado difere do output");

  free_training_example(bad);
  free_training_example(example);
  free_network(network);
}

static void test_backprop_and_free(void) {
  int ok = free_training_example(NULL);
  expect_true(ok == 0, "free_training_example retorna 0 para NULL");

  ok = backpropagation_training_example(NULL, NULL);
  expect_true(ok == 0, "backpropagation retorna 0 (não implementado)");
}

int main(void) {
  srand(1234);

  test_create_training_example();
  test_feedforward_training_example();
  test_get_error_for_training_example();
  test_backprop_and_free();

  if (g_failures != 0) {
    printf("\nResultado: %d teste(s) falharam.\n", g_failures);
    return EXIT_FAILURE;
  }

  printf("\nResultado: todos os testes passaram.\n");
  return EXIT_SUCCESS;
}
