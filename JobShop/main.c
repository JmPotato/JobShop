#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IN.h"
#include "utils.h"
#include "evolution.h"

int main() {
    srand(clock());

    FILE * input_txt;

    if ((input_txt = fopen("input.txt", "r")) == NULL) {
        fprintf(stdout, "Failed to read the input.txt\n");
        exit(EXIT_FAILURE);
    }

    int * scale = readIuputHeader(input_txt);
    int n = *scale, m = *(scale + 1);
    ITEM * items = initializeIiems(input_txt, n);
    MACHINE * machines = initializeMachines(m, n, items);

    int gene_length = 0;
    for (int i = 0;i < n;i++)
        gene_length += items[i].machine_number;

    int ** population = creatPopulation(items, n, gene_length);

    for (int i = 0; i < 7; ++i) {
        printf("%d ", population[0][i]);
    }

    return 0;
}