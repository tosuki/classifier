#include <stdio.h>
#include "network.h"

int main() {
    t_network* network;

    if (!create_network(&network)) {
        perror("Failed to create network");
        exit(EXIT_FAILURE);
    }

    printf("%1.2f\n", activation_function(30.0));

    return EXIT_SUCCESS;
}