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

unsigned int getExeTime(int * gene, int gene_length, ITEM * items, int n, int m) {
    unsigned int * counter = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(counter, 0, n * sizeof(unsigned int));
    unsigned int * machine_end_time = (unsigned int *) malloc(m * sizeof(unsigned int));
    memset(machine_end_time, 0, m * sizeof(unsigned int));
    unsigned int * job_end_time = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(job_end_time, 0, n * sizeof(unsigned int));
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

void decodeGene(int * gene, int gene_length, ITEM * items, int n, MACHINE * machines, int m) {
    for (int i = 0;i < m;i++) {
        machines[i].item_list = (int **) malloc(n * sizeof(int *));
        for (int j = 0;j < n;j++) {
            machines[i].item_list[j] = (int *) malloc(4 * sizeof(int));
            memset(machines[i].item_list[j], 0, 4 * sizeof(unsigned int));
        }
    }

    unsigned int * counter = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(counter, 0, n * sizeof(unsigned int));
    unsigned int * machine_end_time = (unsigned int *) malloc(m * sizeof(unsigned int));
    memset(machine_end_time, 0, m * sizeof(unsigned int));
    unsigned int * job_end_time = (unsigned int *) malloc(n * sizeof(unsigned int));
    memset(job_end_time, 0, n * sizeof(unsigned int));
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
        int index = 0;
        while (machines[machine].item_list[index][3]  != 0)
            index++;

        machines[machine].item_list[index][0] = start_time;
        machines[machine].item_list[index][1] = item;
        machines[machine].item_list[index][2] = operation;
        machines[machine].item_list[index][3] = end_time;
    }

    FILE * output;
    output = fopen("output.txt", "w");

    for (int i = 0;i < m;i++) {
        printf("M%d", i);
        fprintf(output, "M%d", i);
        for (int j = 0;j < machines[i].item_number;j++) {
            printf(" (%d,%d-%d,%d)", machines[i].item_list[j][0], machines[i].item_list[j][1], machines[i].item_list[j][2], machines[i].item_list[j][3]);
            fprintf(output, " (%d,%d-%d,%d)", machines[i].item_list[j][0], machines[i].item_list[j][1], machines[i].item_list[j][2], machines[i].item_list[j][3]);
        }
        printf("\n");
        fprintf(output, "\n");
    }
    fclose(output);

    free(counter);
    free(machine_end_time);
    free(job_end_time);
}