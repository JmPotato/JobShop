//
// Created by JmPotato on 2018/5/14.
//
#include "IN.h"
#include "utils.h"
#include "evolution.h"

#include <stdio.h>
#include <stdlib.h>

#define MaxPopulation 100

int getGeneLength(ITEM * items, int n) {
    int gene_length = 0;
    for (int i = 0;i < n;i++)
        gene_length += items[i].machine_number;

    return gene_length;
}

int * creatGene(ITEM * items, int n) {
    int gene_length = getGeneLength(items, n);
    int * gene = (int *) malloc(gene_length * sizeof(int));
    int k = 0;
    for (int i = 0;i < gene_length;i++) {
        for (int j = 0;j < items[i].machine_number;j++) {
            gene[k] = i + 1;
            k++;
        }
    }

    for (int i = 0;i < gene_length;i++) {
        int index = randomInterger(0, gene_length-1);
        if (index != i) {
            int temp = gene[i];
            gene[i] = gene[index];
            gene[index] = temp;
        }
    }

    return gene;
}


int ** creatPopulation(ITEM * items, int n) {
    int gene_length = getGeneLength(items, n);
    int ** population = (int **) malloc(MaxPopulation * sizeof(int *));
    for (int i = 0;i < MaxPopulation;i++)
        population[i] = creatGene(items, n);

    return population;
}