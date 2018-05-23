#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "IN.h"
#include "utils.h"
#include "evolution.h"

#define MaxThread 5
#define Iteration 20000
#define MaxPopulation 100

struct thread_data {
    int thread_id;
    ITEM * items;
    int n;
    int m;
    int gene_length;
    int * result;
};

void * evolution(void * data);

int main() {
    srand(clock());
    clock_t start_t, end_t;
    start_t = clock();

    FILE * input_txt;

	if ((input_txt = fopen("input-4.txt", "r")) == NULL) {
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

    int result[MaxThread] = {0};

    pthread_t threads[MaxThread];
    struct thread_data data[MaxThread];

    for(int i = 0;i < MaxThread; i++ ){
        data[i].thread_id = i;
        data[i].items = initializeIiems(input_txt, n);;
        data[i].n = n;
        data[i].m = m;
        data[i].gene_length =gene_length;
        data[i].result = &result[i];
        int rc = pthread_create(&threads[i], NULL, evolution, (void *)&data[i]);
        if (rc){
            exit(-1);
        }
    }

    pthread_join(threads[MaxThread-1],NULL);
    int optima = result[0];
    for(int i = 1;i < MaxThread;i++)
        if (result[i])
            optima = result[i] < optima ? result[i] : optima;

    printf("%d\n", optima);
    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time: %lf\n", total_t);

    free(machines);
    free(items);

    pthread_exit(NULL);
}

void * evolution(void * data) {
    struct thread_data *_data;
    _data = (struct thread_data *) data;
    ITEM * items = _data->items;
    int n = _data->n;
    int m = _data->m;
    int gene_length = _data->gene_length;
    int * result = _data->result;
    int ** population = creatPopulation(items, n, gene_length, MaxPopulation);
    int elite_time = getExeTime(population[0], gene_length, items, n, m);
    for (int i = 1;i < MaxPopulation;i++) {
        elite_time = (getExeTime(population[i], gene_length, items, n, m)) < elite_time ? (getExeTime(population[0], gene_length, items, n, m)) : elite_time;
    }
    for (int i = 0; i < Iteration; i++) {
        int **selectedGenes = rouletteGene(population, gene_length, items, n, m, MaxPopulation);
        unsigned int father_time[2] = {0};
        father_time[0] = getExeTime(selectedGenes[0], gene_length, items, n, m);
        father_time[1] = getExeTime(selectedGenes[1], gene_length, items, n, m);
        int *childGene = crossoverGene(selectedGenes, gene_length);
        childGene = mutateGene(childGene, gene_length);
        unsigned int child_time = getExeTime(childGene, gene_length, items, n, m);
        if (child_time <= father_time[0])
            memcpy(selectedGenes[0], childGene, gene_length * sizeof(int));
        if (child_time <= father_time[1])
            memcpy(selectedGenes[1], childGene, gene_length * sizeof(int));
        elite_time = child_time < elite_time ? child_time : elite_time;
        free(childGene);
        free(selectedGenes);
        *result = elite_time;
    }
    free(population);
    pthread_exit(NULL);
}