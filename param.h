#ifndef FS_H
#define FS_H

typedef struct network_param {
    double *params;
    int params_len;
    int params_allocated_len;
} t_network_param;

int create_network_param(t_network_param **param_ptr, const double *params,
                         int params_len);
int write_param_on_file(const t_network_param *param, const char *file_path);
int read_param_from_file(t_network_param **param_ptr, const char *file_path);

#endif