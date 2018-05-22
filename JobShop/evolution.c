//
// Created by JmPotato on 2018/5/14.
//
#include "IN.h"
#include "utils.h"
#include "evolution.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int ** creatPopulation(ITEM * items, int n, int gene_length, int MaxPopulation) {
    int ** population = (int **) malloc(MaxPopulation * sizeof(int *));
    for (int x = 0;x < MaxPopulation;x++) {
        population[x] = (int *) malloc(gene_length * sizeof(int));
        int k = 0;
        for (int i = 0;i < n;i++) {
            for (int j = 0;j < items[i].machine_number;j++) {
                population[x][k] = i + 1;
                k++;
            }
        }

        for (int j = 0;j < gene_length;j++) {
            int index = randomInterger(0, gene_length-1);
            if (index != j) {
                int temp = population[x][j];
                population[x][j] = population[x][index];
                population[x][index] = temp;
            }
        }
    }

    return population;
}

int ** rouletteGene(int ** population, int gene_length, ITEM * items, int n, int m, int MaxPopulation){
	double * probability = (double *) malloc((MaxPopulation + 1) * sizeof(double));
    //[MaxPopulation+1] = { 0 };
    memset(probability, 0.0, n * sizeof(double));
	double randomNum = 0.0;
    double sumFitness = 0.0;
	int ** selectedGenes = (int **)malloc(2 * sizeof(int *));
	for (int j = 1; j <= MaxPopulation; j++) {
		probability[j] = exp((double)1 / getExeTime(population[j-1], gene_length, items, n, m));
		sumFitness += probability[j];
	}
	for (int j = 0; j <= MaxPopulation; j++) {
		if (j == 0) {
			probability[j] = (probability[j] / sumFitness);
		}
		else {
			probability[j] = probability[j - 1] + (probability[j] / sumFitness);
		}
	}
	for (int k = 0; k < 2; k++) {
		randomNum = rand() / (double)RAND_MAX;
		for (int x = 0;x < (MaxPopulation + 1); x++) {
			if (probability[x] <= randomNum && randomNum <= probability[x+1]) {
				selectedGenes[k] = population[x];
                break;
			}
		}
	}
    free(probability);
	return selectedGenes;
}

int * crossoverGene(int ** selectedGenes, int gene_length) {
	int left = 0, right = 0;
	int judge = 1;
	int temp = 0;
	int *childGene = (int *) malloc(gene_length * sizeof(int));
	int ***indexOfGene = (int ***) malloc(2 * sizeof(int **));
	for (int count = 0; count < 2; count++) {
		indexOfGene[count] = (int **) malloc(gene_length * sizeof(int *));
		for (int i = 0; i < gene_length; i++) {
			int countA = 0;
			indexOfGene[count][i] = (int *) malloc(2 * sizeof(int));
			indexOfGene[count][i][0] = selectedGenes[count][i];
			for (int j = 0; j <= i; j++) {
				if (selectedGenes[count][j] == selectedGenes[count][i]) {
					countA++;
					indexOfGene[count][i][1] = countA;
				}
			}
		}
	}
	while (judge) {
		left = randomInterger(0, gene_length - 1);
		right = randomInterger(0, gene_length - 1);
		if ((left != right) && (left != 0 && right != 6)) {
			judge = 0;
		}
	}
	if (left > right) {
		temp = left;
		left = right;
		right = temp;
	}
	for (int count = left; count <= right; count++) {
		childGene[count] = indexOfGene[0][count][0];
	}
	int tempLength = gene_length;
    for (int i = left; i <= right; i++) {
        for (int j = 0; j < tempLength; j++) {
            if ((indexOfGene[1][j][0] == indexOfGene[0][i][0]) && (indexOfGene[1][j][1] == indexOfGene[0][i][1])) {
                if (j != tempLength - 1) {
                    for (int r = j; r < tempLength - 1; r++) {
                        memcpy(indexOfGene[1][r], indexOfGene[1][r + 1], 2 * sizeof(int));
                    }
                    tempLength--;
                }
                else{
                    tempLength--;
                }
            }
        }
    }
    int m, n;
    for (m = 0; m < left; m++)
        childGene[m] = indexOfGene[1][m][0];
    for (n = right + 1; n < gene_length; n++, m++)
        childGene[n] = indexOfGene[1][m][0];

	return childGene;
}

int * mutateGene(int * childGene, int gene_length) {
    int index_1 = randomInterger(0, gene_length-1);
    int index_2 = randomInterger(0, gene_length-1);
    while (index_2 == index_1)
        index_2 = randomInterger(0, gene_length-1);

    int temp = childGene[index_1];
    childGene[index_1] = childGene[index_2];
    childGene[index_2] = temp;

    return childGene;

}