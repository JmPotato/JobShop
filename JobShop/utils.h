//
// Created by JmPotato on 2018/5/9.
//

#ifndef JOBSHOP_UTILS_H
#define JOBSHOP_UTILS_H

int randomInterger(int a, int b);
void decodeGene(int * gene, int gene_length, ITEM * items, int n, MACHINE * machines, int m);
unsigned int getExeTime(int * gene, int gene_length, ITEM * items, int n, int m);

#endif //JOBSHOP_UTILS_H
