# Project Specification: Classifier

## Overview
**Classifier** is a simple neural network implementation in C (C11). It provides a foundational structure for creating, managing, and visualizing a multi-layer perceptron.

## Project Structure
The project follows a modular structure:
- `main.c`: Entry point demonstrating network initialization and layer creation.
- `network.h`: Header file containing data structures (`t_network`, `t_neuron_layer`, `t_neuron`) and function prototypes.
- `network.c`: Implementation of network management logic, memory allocation, and mathematical functions.
- `Makefile`: Build system configuration.

## Data Structures

### `t_neuron`
Represents an individual unit in the network.
- `double value`: The current activation value of the neuron.
- `double *weights`: Pointer to an array of weights connected to the previous layer.
- `int weights_len`: Number of incoming weights.

### `t_neuron_layer`
Represents a collection of neurons.
- `int nodes_len`: Number of neurons in the layer.
- `t_neuron **neurons`: Array of pointers to neurons.

### `t_network`
The main container for the neural network.
- `t_neuron_layer **layers`: Array of pointers to layers.
- `int layers_len`: Current number of layers.
- `int layers_allocated_mem`: Capacity of the layers array.

## Core Functionality

### Network Management
- `create_network`: Initializes a new network structure.
- `create_neuron_layer`: Dynamically adds a new layer to the network, initializing neurons and randomizing weights based on the previous layer's size.
- `free_network`: Recursively frees all allocated memory for the network, layers, neurons, and weights.

### Visualization
- `print_network`: Outputs a formatted visual representation of the network's structure, including layer types (Input, Hidden, Output), neuron values, and weights.

### Mathematics
- `activation_function`: Implements the Sigmoid function: $f(x) = \frac{1}{1 + e^{-x}}$.

## Build and Execution

### Prerequisites
- GCC Compiler
- GSL (GNU Scientific Library) - *Linked in Makefile*

### Commands
- `make`: Compiles the project and generates the `classifier` executable.
- `make run`: Compiles and executes the program.
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Performs a full re-build.

## Future Enhancements
- Implementation of forward propagation logic.
- Support for backpropagation and training.
- Integration with GSL for optimized matrix operations.
- Support for different activation functions (ReLU, Tanh).
