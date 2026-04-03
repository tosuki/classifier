# Classifier - Simple Neural Network in C

**Classifier** é uma implementação educacional de uma rede neural Multi-Layer Perceptron (MLP) escrita em **C11**. O projeto utiliza a biblioteca **GSL (GNU Scientific Library)** para operações de álgebra linear, garantindo cálculos matriciais eficientes para a propagação de valores entre camadas.

## Funcionalidades

- **Estrutura Modular:** Gerenciamento dinâmico de redes, camadas e neurônios.
- **Álgebra Linear:** Uso de `gsl_matrix` e `gsl_blas` para cálculos de feedforward.
- **Função de Ativação:** Implementação da função Sigmoid para normalização de outputs.
- **Visualização:** Impressão elegante da estrutura da rede, valores e pesos no terminal.
- **Gerenciamento de Memória:** Limpeza recursiva e segura de todos os recursos alocados.

## Estrutura do Projeto

- `main.c`: Script de teste que inicializa a rede e executa a propagação.
- `network.c`: Implementação core da lógica da rede e cálculos matriciais.
- `network.h`: Definições de tipos (structs) e protótipos de funções.
- `Makefile`: Sistema de build automatizado.
- `project_spec.md`: Especificação técnica detalhada.

## Pré-requisitos

Para compilar este projeto, você precisará de:

- **GCC** (ou qualquer compilador compatível com C11).
- **GSL (GNU Scientific Library)**:
  - No Ubuntu/Debian: `sudo apt-get install libgsl-dev`
  - No macOS (Homebrew): `brew install gsl`

## Como Usar

1. **Compilar o projeto:**

    ```bash
    make
    ```

2. **Executar os testes:**

    ```bash
    make run
    ```

3. **Limpar arquivos temporários:**

    ```bash
    make clean
    ```

4. **Remover tudo (incluindo o executável):**

    ```bash
    make fclean
    ```

## Exemplo de Saída

Ao rodar o projeto, você verá uma representação visual da rede antes e depois do cálculo de propagação:

```text
--- Neural Network Structure ---

[Layer 0] (Input)
  ○ Neuron 0: Value: 1.000
  ○ Neuron 1: Value: 0.500

[Layer 1] (Hidden)
  ○ Neuron 0: Value: 0.000
    └── Weights: [ 0.234, -0.412 ]
  ...
```

## Roadmap de Desenvolvimento

- [ ] Implementação de Forward Propagation completo (várias camadas sequenciais).
- [ ] Adição de suporte para Backpropagation e treinamento.
- [ ] Implementação de Bias em cada neurônio.
- [ ] Suporte para diferentes funções de ativação (ReLU, Tanh).
- [ ] Carregamento/Salvamento de modelos a partir de arquivos JSON ou binários.
