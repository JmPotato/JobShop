//
// Created by JmPotato on 2018/5/9.
//

#ifndef JOBSHOP_EVOLUTION_H
#define JOBSHOP_EVOLUTION_H

int * creatGene(ITEM * items, int n, int gene_length);
int ** creatPopulation(ITEM * items, int n, int gene_length);
int * mutateGene();
int * reproductGene();

#endif //JOBSHOP_EVOLUTION_H
