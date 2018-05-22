#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IN.h"
#include "utils.h"
#include "evolution.h"

#define Iteration 18000
#define MaxPopulation 1000

int main() {
    srand(clock());
    clock_t start_t, end_t;
    start_t = clock();

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

    int ** population = creatPopulation(items, n, gene_length, MaxPopulation);
    int elite_time = getExeTime(population[0], gene_length, items, n, m);
    for (int i = 1;i < MaxPopulation;i++) {
        elite_time = (getExeTime(population[i], gene_length, items, n, m)) < elite_time ? (getExeTime(population[0], gene_length, items, n, m)) : elite_time;
    }

    for (int i = 0;i < Iteration;i++) {
        int ** selectedGenes = rouletteGene(population, gene_length, items, n, m, MaxPopulation);
        unsigned int father_time[2] = {0};
        father_time[0] = getExeTime(selectedGenes[0], gene_length, items, n, m);
        father_time[1] = getExeTime(selectedGenes[1], gene_length, items, n, m);
        int * childGene = crossoverGene(selectedGenes,gene_length);
        childGene = mutateGene(childGene, gene_length);
        unsigned int child_time = getExeTime(childGene, gene_length, items, n, m);
        if (child_time <= father_time[0])
            memcpy(selectedGenes[0], childGene, gene_length * sizeof(int));
        if (child_time <= father_time[1])
            memcpy(selectedGenes[1], childGene, gene_length * sizeof(int));
        elite_time = child_time < elite_time ? child_time : elite_time;
        free(selectedGenes);
        free(childGene);
        printf("%d\n", elite_time);
    }

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time: %lf\n", total_t);

    free(items);
    free(machines);
    free(population);

    return 0;
}