#include "param.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

static int g_failures = 0;

static void free_network_param_data(t_network_param *param) {
  if (!param) {
    return;
  }

  free(param->params);
  free(param);
}

static void expect_true(int condition, const char *message) {
  if (condition) {
    printf("[OK]   %s\n", message);
  } else {
    printf("[FAIL] %s\n", message);
    g_failures++;
  }
}

static int almost_equal(double a, double b) {
  return fabs(a - b) < 1e-12;
}

static void test_create_network_param_success(void) {
  t_network_param *param = NULL;
  double values[] = {1.0, 2.0, 3.5};

  errno = 0;
  int ok = create_network_param(&param, values, 3);

  expect_true(ok == 1, "create_network_param cria estrutura válida");
  expect_true(param != NULL, "ponteiro de saída foi preenchido");
  expect_true(param->params_len == 3, "params_len definido corretamente");
  expect_true(param->params_allocated_len == 3,
              "params_allocated_len definido corretamente");
  expect_true(almost_equal(param->params[0], 1.0), "valor[0] copiado");
  expect_true(almost_equal(param->params[1], 2.0), "valor[1] copiado");
  expect_true(almost_equal(param->params[2], 3.5), "valor[2] copiado");

  free_network_param_data(param);
}

static void test_create_network_param_invalid_args(void) {
  t_network_param *param = NULL;
  double values[] = {1.0};

  errno = 0;
  int ok = create_network_param(NULL, values, 1);
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "create_network_param falha com param_ptr NULL");

  errno = 0;
  ok = create_network_param(&param, NULL, 1);
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "create_network_param falha com params NULL");

  errno = 0;
  ok = create_network_param(&param, values, 0);
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "create_network_param falha com len inválido");
}

static void test_write_and_read_roundtrip(void) {
  const char *path = "params_test.bin";
  t_network_param *written = NULL;
  t_network_param *read_back = NULL;
  double values[] = {10.0, -2.25, 0.125, 99.0};

  errno = 0;
  int ok = create_network_param(&written, values, 4);
  expect_true(ok == 1, "criação de params para roundtrip");

  errno = 0;
  ok = write_param_on_file(written, path);
  expect_true(ok == 1, "write_param_on_file grava arquivo binário");

  errno = 0;
  ok = read_param_from_file(&read_back, path);
  expect_true(ok == 1, "read_param_from_file lê arquivo binário");

  if (ok && read_back) {
    expect_true(read_back->params_len == 4, "roundtrip preserva params_len");
    expect_true(almost_equal(read_back->params[0], values[0]),
                "roundtrip preserva valor[0]");
    expect_true(almost_equal(read_back->params[1], values[1]),
                "roundtrip preserva valor[1]");
    expect_true(almost_equal(read_back->params[2], values[2]),
                "roundtrip preserva valor[2]");
    expect_true(almost_equal(read_back->params[3], values[3]),
                "roundtrip preserva valor[3]");
  }

  remove(path);
  free_network_param_data(written);
  free_network_param_data(read_back);
}

static void test_io_invalid_args(void) {
  t_network_param *param = NULL;
  double values[] = {5.0};

  create_network_param(&param, values, 1);

  errno = 0;
  int ok = write_param_on_file(NULL, "x.bin");
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "write_param_on_file falha com param NULL");

  errno = 0;
  ok = write_param_on_file(param, NULL);
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "write_param_on_file falha com path NULL");

  errno = 0;
  ok = read_param_from_file(NULL, "x.bin");
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "read_param_from_file falha com destino NULL");

  errno = 0;
  ok = read_param_from_file(&param, NULL);
  expect_true(ok == 0 && errno == INVALID_PARAMS,
              "read_param_from_file falha com path NULL");

  free_network_param_data(param);
}

static void test_read_missing_file(void) {
  t_network_param *param = NULL;

  errno = 0;
  int ok = read_param_from_file(&param, "arquivo_inexistente_12345.bin");
  expect_true(ok == 0 && errno == FS_ERROR,
              "read_param_from_file falha ao abrir arquivo inexistente");
}

int main(void) {
  test_create_network_param_success();
  test_create_network_param_invalid_args();
  test_write_and_read_roundtrip();
  test_io_invalid_args();
  test_read_missing_file();

  if (g_failures != 0) {
    printf("\nResultado: %d teste(s) falharam.\n", g_failures);
    return EXIT_FAILURE;
  }

  printf("\nResultado: todos os testes passaram.\n");
  return EXIT_SUCCESS;
}