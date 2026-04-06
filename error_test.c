#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_failures = 0;

static void expect_true(int condition, const char *message) {
  if (condition) {
    printf("[OK]   %s\n", message);
  } else {
    printf("[FAIL] %s\n", message);
    g_failures++;
  }
}

static void expect_string_eq(const char *actual, const char *expected,
                             const char *message) {
  expect_true(strcmp(actual, expected) == 0, message);
}

static void test_known_error_messages(void) {
  expect_string_eq(get_error_message(MEM_ERROR), "Failed to allocate memory",
                   "MEM_ERROR retorna mensagem correta");
  expect_string_eq(get_error_message(INVALID_PARAMS), "Invalid parameters",
                   "INVALID_PARAMS retorna mensagem correta");
  expect_string_eq(get_error_message(FS_ERROR), "Failed to open file",
                   "FS_ERROR retorna mensagem correta");
  expect_string_eq(get_error_message(FS_FAILED_TO_WRITE),
                   "Failed to write on file",
                   "FS_FAILED_TO_WRITE retorna mensagem correta");
  expect_string_eq(get_error_message(FS_FAILED_TO_READ),
                   "Failed to read from file",
                   "FS_FAILED_TO_READ retorna mensagem correta");
}

static void test_unknown_error_message(void) {
  expect_string_eq(get_error_message(9999), "Unhandled",
                   "erro desconhecido retorna mensagem padrão");
}

int main(void) {
  test_known_error_messages();
  test_unknown_error_message();

  if (g_failures != 0) {
    printf("\nResultado: %d teste(s) falharam.\n", g_failures);
    return EXIT_FAILURE;
  }

  printf("\nResultado: todos os testes passaram.\n");
  return EXIT_SUCCESS;
}
