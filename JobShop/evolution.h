//
// Created by JmPotato on 2018/5/9.
//

#ifndef JOBSHOP_EVOLUTION_H
#define JOBSHOP_EVOLUTION_H

int ** creatPopulation(ITEM * items, int n, int gene_length, int MaxPopulation);
int ** rouletteGene(int ** population, int gene_length, ITEM * items, int n, int m, int MaxPopulation);
int * crossoverGene(int ** selectedGenes, int gene_length);
int * mutateGene(int * childGene, int gene_length);

#endif //JOBSHOP_EVOLUTION_H
