//
// Created by JmPotato on 2018/5/14.
//
#include "IN.h"
#include "utils.h"

#include <time.h>
#include <stdlib.h>
#include <memory.h>

int randomInterger(int a, int b) {
    int interger = (rand() % (b-a+1)) + a;
    return interger;
}

void decodeGene(int * gene, int gene_length, ITEM * items, int n, MACHINE * machines, int m) {
    int * counter = (int *) malloc(n * sizeof(int));
    memset(counter, 0, n);

    for (int i = 0;i < m;i++) {
        int index = 0;
        for (int j = 0;j < gene_length;j++) {
            if (counter[gene[j]-1] == i) {
                machines[i].item_list[index] = gene[j];
                counter[gene[j]-1]++;
            }
        }
    }

    free(counter);
}

unsigned int getExeTime(int * gene, int gene_length, ITEM * items, int n, int m) {
    unsigned int * counter = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(counter, 0, n);
    unsigned int * machine_end_time = (unsigned int *) malloc(m * sizeof(unsigned int));
    memset(machine_end_time, 0, m);
    unsigned int * job_end_time = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(job_end_time, 0, n);
    unsigned int start_time = 0;
    unsigned int end_time = 0;
    unsigned int sum_time = 0;
    unsigned int item = 0;
    unsigned int operation = 0;
    unsigned int machine = 0;

    for (int i = 0;i < gene_length;i++) {
        item = gene[i]-1;
        counter[item]++;
        operation = counter[item] - 1;
        machine = items[item].schedule[operation][1] - 1;
        start_time = machine_end_time[machine] > job_end_time[item] ? machine_end_time[machine] : job_end_time[item];
        end_time = start_time + items[item].schedule[operation][0];
        machine_end_time[machine] = end_time;
        job_end_time[item] = end_time;
        sum_time = sum_time > end_time ? sum_time : end_time;
    }

    free(counter);
    free(machine_end_time);
    free(job_end_time);

    return sum_time;
}