//
// Created by JmPotato on 2018/5/14.
//
#include "IN.h"
#include "utils.h"

#include <time.h>
#include <stdlib.h>

int randomInterger(int a, int b) {

    int interger = (rand() % (b-a+1)) + a;

    return interger;
}

void * decodeGene(int * gene, int gene_length, ITEM * items, int n, MACHINE * machines, int m) {
    int counter[n] = {0};

    for (int i = 0;i < m;i++) {
        int index = 0;
        for (int j = 0;j < gene_length;j++) {
            if (counter[gene[j]-1] == i) {
                machines[i].item_list[index] = gene[j];
                counter[gene[j]-1]++;
            }
        }
    }
}

unsigned int getExeTime(ITEM * items, int n, MACHINE * machines, int m) {
    unsigned int time = 0;

    return time;
}