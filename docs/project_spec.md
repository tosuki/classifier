# Project Specification: Classifier

## Overview
**Classifier** é uma implementação educacional de uma rede neural Multi-Layer Perceptron (MLP) em C11, com suporte a gerenciamento dinâmico de redes, camadas e neurônios. O projeto utiliza a **GSL (GNU Scientific Library)** para operações matriciais eficientes e inclui tratamento padronizado de erros, I/O binário e suítes completas de testes unitários.

## Estrutura do Projeto

```
classifier/
├── main.c                  # Exemplo de inicialização da rede
├── network.c/h             # Core da rede neural
├── training.c/h            # Lógica de treinamento
├── param.c/h               # Gerenciamento de parâmetros
├── error.c/h               # Tratamento de erros padronizado
├── param_test.c            # Testes unitários (param)
├── error_test.c            # Testes unitários (error)
├── network_test.c          # Testes unitários (network)
├── training_test.c         # Testes unitários (training)
├── Makefile                # Build system
├── .gitignore              # Git configuration
└── docs/
    └── project_spec.md     # Este arquivo
```

## Estruturas de Dados

### `t_neuron` (network.h)
Representa um neurônio individual na rede.
```c
typedef struct neuron {
    double value;           // Valor de ativação (a = sigmoid(z))
    double *weights;        // Pesos para conexão com camada anterior
    int weights_len;        // Número de pesos
} t_neuron;
```

**TODO**: Adicionar campos para backpropagation:
- `double z` — saída linear pré-ativação
- `double bias` — termo independente
- `double grad_w`, `double grad_b` — gradientes

### `t_neuron_layer` (network.h)
Representa uma camada de neurônios.
```c
typedef struct neuron_layer {
    int nodes_len;
    t_neuron **neurons;     // Array dinâmico de neurônios
} t_neuron_layer;
```

### `t_network` (network.h)
Container principal para a rede neural.
```c
typedef struct network {
    t_neuron_layer **layers;        // Array dinâmico de camadas
    int layers_len;                 // Número atual de camadas
    int layers_allocated_mem;       // Capacidade alocada
} t_network;
```

### `t_network_param` (param.h)
Estrutura para gerenciar parâmetros da rede.
```c
typedef struct network_param {
    double *params;
    int params_len;
    int params_allocated_len;
} t_network_param;
```

### `t_training_example` (training.h)
Estrutura para dados de treinamento.
```c
typedef struct training_example {
    int expected_output_len;
    int expected_output_allocated_mem;
    int expected_output_len_used;
    int *expected_output;
} t_training_example;
```

## Sistema de Tratamento de Erros

Todos os módulos seguem o padrão unificado definido em `error.h` e `error.c`:

```c
#define MEM_ERROR 3              // Falha de alocação de memória
#define INVALID_PARAMS 5         // Parâmetros inválidos
#define FS_ERROR 6               // Erro ao abrir arquivo
#define FS_FAILED_TO_WRITE 7     // Falha ao escrever em arquivo
#define FS_FAILED_TO_READ 8      // Falha ao ler de arquivo
```

**Padrão de uso**:
```c
if (error_condition) {
    errno = ERROR_CODE;
    return 0;
}
```

Recuperar mensagem de erro:
```c
printf("Error: %s\n", get_error_message(errno));
```

## Funcionalidades Implementadas

### Network Management (network.c/h)
- `int create_network(t_network **network)` — Aloca e inicializa rede
- `int create_neuron_layer(t_network *network, t_neuron_layer **layer, const double *values, int values_len)` — Cria camada com neurônios
- `int calculate_layer_output(t_neuron_layer *layer, const t_neuron_layer *prev_layer)` — Executa feedforward com GSL
- `void free_network(t_network *network)` — Libera toda memória recursivamente
- `void print_network(const t_network *network)` — Exibe estrutura da rede

### Parameter Management (param.c/h)
- `int create_network_param(t_network_param **param_ptr, const double *params, int params_len)` — Cria estrutura de parâmetros
- `int write_param_on_file(const t_network_param *param, const char *file_path)` — Salva em arquivo binário
- `int read_param_from_file(t_network_param **param_ptr, const char *file_path)` — Carrega de arquivo binário
- `int free_network_param(t_network_param **param_ptr)` — Libera parâmetros

### Training Functions (training.c/h)
- `int create_training_example(t_training_example **example, const int *expected_output, int expected_output_len)` — Cria exemplo
- `int feedforward_training_example(t_network *network, const t_training_example *example)` — Executa feedforward com exemplo
- `int get_error_for_training_example(t_network *network, const t_training_example *example, double *error)` — Calcula erro
- `int free_training_example(t_training_example *example)` — Libera exemplo

### Error Handling (error.c/h)
- `char *get_error_message(int error_code)` — Retorna mensagem de erro em português

### Mathematics
- `double activation_function(double value)` — Implementa Sigmoid: $f(x) = \frac{1}{1 + e^{-x}}$

## Suítes de Testes Unitários

O projeto inclui **testes unitários abrangentes** para validar todas as funcionalidades:

### param_test.c
- ✅ Criação de estrutura de parâmetros
- ✅ Validação de argumentos inválidos
- ✅ Roundtrip (escrita/leitura) de arquivo binário
- ✅ Liberação de memória

### error_test.c
- ✅ Mensagens de erro para todos os códigos conhecidos
- ✅ Fallback para erros desconhecidos

### network_test.c
- ✅ Criação de rede e camadas
- ✅ Validação de argumentos
- ✅ Função de ativação Sigmoid
- ✅ Cálculo de saída de camada (feedforward)

### training_test.c
- ✅ Criação de exemplos de treinamento
- ✅ Feedforward com exemplos
- ✅ Cálculo de erro
- ✅ Validações de entrada

**Executar testes**: `make test`

## Build e Execução

### Pré-requisitos
- **GCC** (ou compilador C11 compatível)
- **GSL (GNU Scientific Library)**
  - Ubuntu/Debian: `sudo apt-get install libgsl-dev`
  - macOS: `brew install gsl`

### Comandos
- `make` — Compila o projeto (gera `classifier`)
- `make run` — Compila e executa
- `make test` — Compila e executa todas as suítes de testes
- `make clean` — Remove arquivos objeto
- `make fclean` — Remove arquivos objeto e executáveis
- `make re` — Rebuild completo

## Padrão de Código

### Ponteiros e Const-Correctness
- Funções que leem usam `const` no parâmetro: `calculate_layer_output(t_neuron_layer *layer, const t_neuron_layer *prev_layer)`
- Ponteiros de saída usam duplo indirecionamento: `create_network(t_network **network)`
- Documentação marca quais parâmetros são entrada/saída

### Validação de Entrada
```c
if (!param_ptr || !values || values_len <= 0) {
    errno = INVALID_PARAMS;
    return 0;
}
```

### Limpeza de Memória
- Sempre liberar arrays alocados antes de liberar struct
- Zerar ponteiros após liberar: `*ptr = NULL`
- Usar função dedicada de free quando possível

## Roadmap de Desenvolvimento

- [x] Estrutura modular de rede, camadas e neurônios
- [x] Álgebra linear com GSL (feedforward básico)
- [x] Função de ativação Sigmoid
- [x] Testes unitários (param, error, network, training)
- [x] Carregamento/Salvamento de modelos em arquivo binário
- [x] Tratamento padronizado de erros com errno
- [ ] **Armazenar `z` (saída linear pré-ativação) em cada neurônio**
- [ ] **Adicionar Bias em cada neurônio**
- [ ] Atualizar `calculate_layer_output()` para usar bias e armazenar z
- [ ] Implementação de Backpropagation e cálculo de gradientes
- [ ] Training loop com descida de gradiente
- [ ] Suporte para diferentes funções de ativação (ReLU, Tanh)
- [ ] Persistência de pesos e arquitetura de rede em arquivo binário

## Exemplo de Uso

```c
#include "network.h"
#include "training.h"
#include <time.h>

int main() {
    srand(time(NULL));
    
    t_network *network;
    create_network(&network);
    
    // Criar camada de entrada
    double input_values[] = {1.0, 0.5};
    t_neuron_layer *input_layer;
    create_neuron_layer(network, &input_layer, input_values, 2);
    
    // Criar camada oculta
    double hidden_values[] = {0.0, 0.0, 0.0};
    t_neuron_layer *hidden_layer;
    create_neuron_layer(network, &hidden_layer, hidden_values, 3);
    
    // Executar feedforward
    print_network(network);
    calculate_layer_output(hidden_layer, input_layer);
    print_network(network);
    
    // Limpar
    free_network(network);
    return 0;
}
```

## Notas Arquiteturais

1. **GSL para Álgebra Linear**: Operações matriciais (`gsl_blas_dgemm`) garantem performance em feedforward
2. **Alocação Dinâmica**: Arrays de camadas/neurônios crescem dinamicamente com realloc
3. **Separação de Responsabilidades**: Network (estrutura), Training (lógica), Param (I/O)
4. **Const-Correctness**: Funções de leitura marcam parâmetros como `const`
5. **Tratamento de Erro Unificado**: Uso de `errno` em todo o código

## Contribuindo

Para adicionar novas funcionalidades:
1. Siga o padrão de tratamento de erro existente
2. Adicione testes unitários correspondentes
3. Atualize este arquivo de documentação
4. Execute `make test` antes de fazer commit
