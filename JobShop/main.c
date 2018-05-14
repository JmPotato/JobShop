#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IN.h"

int main() {
    FILE * input_txt;

    if ((input_txt = fopen("input.txt", "r")) == NULL) {
        fprintf(stdout, "Failed to read the input.txt\n");
        exit(EXIT_FAILURE);
    }

    int * scale = readIuputHeader(input_txt);
    ITEM * items = initializeIiems(input_txt, *scale);
    MACHINE * machines = initializeMachines(input_txt, *(scale + 1));

    return 0;
}