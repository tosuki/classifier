#ifndef FS_H
#define FS_H

typedef struct network_param {
    double *params;
    int params_len;
    int params_allocated_len;
} t_network_param;

/**
 * Cria e inicializa uma estrutura de parâmetros da rede.
 *
 * A função aloca memória para `t_network_param` e para o vetor interno de
 * parâmetros, copiando os valores recebidos em `params`.
 *
 * @param param_ptr Ponteiro para o ponteiro de saída onde a estrutura será
 * armazenada. Deve apontar para `NULL` antes da chamada.
 * @param params Vetor de valores a ser copiado para a estrutura.
 * @param params_len Quantidade de elementos em `params`.
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
int create_network_param(t_network_param **param_ptr, const double *params,
                         int params_len);

/**
 * Escreve os parâmetros da rede em arquivo binário.
 *
 * O arquivo é salvo com o tamanho (`params_len`) seguido pelos valores do
 * vetor `params`.
 *
 * @param param Estrutura de parâmetros a ser serializada.
 * @param file_path Caminho do arquivo de destino.
 * @return 1 em caso de sucesso, 0 em caso de falha de I/O ou parâmetros
 * inválidos.
 */
int write_param_on_file(const t_network_param *param, const char *file_path);

/**
 * Lê os parâmetros da rede de um arquivo binário.
 *
 * A função carrega `params_len` e os valores do arquivo, alocando uma nova
 * estrutura em `param_ptr`.
 *
 * @param param_ptr Ponteiro para o ponteiro de saída onde a estrutura lida
 * será armazenada.
 * @param file_path Caminho do arquivo de origem.
 * @return 1 em caso de sucesso, 0 em caso de falha de I/O, memória ou
 * parâmetros inválidos.
 */
int read_param_from_file(t_network_param **param_ptr, const char *file_path);

/**
 * Libera uma estrutura de parâmetros da rede e zera o ponteiro.
 *
 * @param param_ptr Ponteiro para o ponteiro da estrutura a ser liberada.
 * @return 1 em caso de sucesso, 0 para parâmetro inválido.
 */
int free_network_param(t_network_param **param_ptr);

#endif