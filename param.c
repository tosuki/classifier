#include "param.h"
#include "error.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int create_network_param(t_network_param **param_ptr, const double *params,
                         int params_len) {
  if (!param_ptr || (*param_ptr) || !params || params_len < 1) {
    errno = INVALID_PARAMS;
    return 0;
  }

  (*param_ptr) = malloc(sizeof(t_network_param));

  if (!(*param_ptr)) {
    errno = MEM_ERROR;
    return 0;
  }

  (*param_ptr)->params = malloc(sizeof(double) * params_len);

  if (!(*param_ptr)->params) {
    errno = MEM_ERROR;
    free(*param_ptr);
    *param_ptr = NULL;
    return 0;
  }

  for (int i = 0; i < params_len; i++) {
    (*param_ptr)->params[i] = params[i];
  }

  (*param_ptr)->params_allocated_len = params_len;
  (*param_ptr)->params_len = params_len;

  return 1;
}

int write_param_on_file(const t_network_param *param, const char *file_path) {
  if (!param || !file_path) {
    errno = INVALID_PARAMS;
    return 0;
  }

  FILE *fptr = fopen(file_path, "wb");

  if (!fptr) {
    errno = FS_ERROR;
    return 0;
  }

  size_t w = fwrite(&param->params_len, sizeof(int), 1, fptr);

  if (w != 1) {
    errno = FS_FAILED_TO_WRITE;
    fclose(fptr);
    return 0;
  }

  for (int i = 0; i < param->params_len; i++) {
    w = fwrite(&param->params[i], sizeof(double), 1, fptr);

    if (w != 1) {
      errno = FS_FAILED_TO_WRITE;
      fclose(fptr);
      return 0;
    }
  }

  fclose(fptr);
  return 1;
}

int read_param_from_file(t_network_param **param_ptr, const char *file_path) {
  if (!param_ptr || !file_path) {
    errno = INVALID_PARAMS;
    return 0;
  }

  FILE *fptr = fopen(file_path, "rb");

  if (!fptr) {
    errno = FS_ERROR;
    return 0;
  }

  int params_len;

  size_t r = fread(&params_len, sizeof(int), 1, fptr);

  if (r != 1) {
    errno = FS_FAILED_TO_READ;
    fclose(fptr);
    return 0;
  }

  double *params = malloc(sizeof(double) * params_len);

  if (!params) {
    errno = MEM_ERROR;
    fclose(fptr);
    return 0;
  }

  for (int i = 0; i < params_len; i++) {
    r = fread(&params[i], sizeof(double), 1, fptr);

    if (r != 1) {
      errno = FS_FAILED_TO_READ;
      free(params);
      fclose(fptr);
      return 0;
    }
  }

  fclose(fptr);

  int res = create_network_param(param_ptr, params, params_len);
  free(params);
  return res;
}

int free_network_param(t_network_param **param_ptr) {
  if (!param_ptr || !(*param_ptr)) {
    errno = INVALID_PARAMS;
    return 0;
  }

  free((*param_ptr)->params);
  free(*param_ptr);
  *param_ptr = NULL;

  return 1;
}
