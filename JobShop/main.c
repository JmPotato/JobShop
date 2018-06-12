#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <pthread.h>

#include "IN.h"
#include "utils.h"
#include "evolution.h"

//#define MaxThread 1 //多线程最大线程数
#define Iteration 5000 //进化的迭代次数
#define MaxPopulation 100 //最大种群个体数

/*多线程函数传参用结构体
struct thread_data {
    ITEM * items;
    int n;
    int m;
    int gene_length;
    int * result;
};

void * evolution(void * data);
*/

int main() {
    srand(clock());
    clock_t start_t, end_t;

    FILE * input, * output;

    /*
	if ((input = fopen("input-2.txt", "r")) == NULL) {
        fprintf(stdout, "Failed to read the input.txt\n");
        exit(EXIT_FAILURE);
    }*/

    input = stdin; //重定向输入

    /*读取样例，初始化数据结构*/
    int * scale = readIuputHeader(input);
    int n = *scale, m = *(scale + 1);
    ITEM * items = initializeIiems(input, n);
    MACHINE * machines = initializeMachines(m, n, items);

    /*计算基因长度*/
    int gene_length = 0;
    for (int i = 0;i < n;i++)
        gene_length += items[i].machine_number;

    /*
    int result[MaxThread] = {0};
    pthread_t threads[MaxThread];
    struct thread_data data[MaxThread];

    start_t = clock();
    for(int i = 0;i < MaxThread; i++ ){
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

    pthread_join(threads[0],NULL);
    int optima = result[0];
    for(int i = 1;i < MaxThread;i++)
        if (result[i])
            optima = result[i] < optima ? result[i] : optima;

    printf("Result: %d\n", optima);*/

    /*生成种群*/
    int ** population = creatPopulation(items, n, gene_length, MaxPopulation);
    int elite_time = getExeTime(population[0], gene_length, items, n, m);
    int elite_gene[gene_length];
    for (int i = 1;i < MaxPopulation;i++) {
        elite_time = (getExeTime(population[i], gene_length, items, n, m)) < elite_time ? (getExeTime(population[0], gene_length, items, n, m)) : elite_time;
    }
    start_t = clock();
    /*进行迭代进化*/
    for (int i = 0; i < Iteration; i++) {
        int ** selectedGenes = rouletteGene(population, gene_length, items, n, m, MaxPopulation);
        unsigned int father_time[2] = {0};
        /*轮盘赌选择基因*/
        father_time[0] = getExeTime(selectedGenes[0], gene_length, items, n, m);
        father_time[1] = getExeTime(selectedGenes[1], gene_length, items, n, m);
        int * childGene = crossoverGene(selectedGenes, gene_length); //OX 顺序交叉基因
        childGene = mutateGene(childGene, gene_length); //进行基因突变
        unsigned int child_time = getExeTime(childGene, gene_length, items, n, m); //计算当前子代的最短完工时间
        if (child_time > elite_time) {
            if (child_time <= father_time[0])
                memcpy(selectedGenes[0], childGene, gene_length * sizeof(int));
            if (child_time <= father_time[1])
                memcpy(selectedGenes[1], childGene, gene_length * sizeof(int));
        } else {
            elite_time = child_time;
            memcpy(elite_gene, childGene, gene_length * sizeof(int));
        }
        free(childGene);
        free(selectedGenes);
    }
    free(population);
    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    /*结果的命令行输出*/
    printf("运行结果：\n");
    decodeGene(elite_gene, gene_length, items, n, machines, m); //解码基因，转化为具体的工序
    printf("程序运行时间: %lfs\n", total_t);
    printf("最短完工时间: %d\n", elite_time);

    /*结果的文件输出*/
    output = fopen("output.txt", "r+");
    fseek(output, 0, SEEK_END);
    fprintf(output, "Time Used: %lfs\n", total_t);
    fprintf(output, "End Time: %d", elite_time);
    fclose(output);

    free(machines);
    free(items);

    //pthread_exit(NULL);

    return 0;
}

/*多线程遗留代码*/
/*
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
        int ** selectedGenes = rouletteGene(population, gene_length, items, n, m, MaxPopulation);
        unsigned int father_time[2] = {0};
        father_time[0] = getExeTime(selectedGenes[0], gene_length, items, n, m);
        father_time[1] = getExeTime(selectedGenes[1], gene_length, items, n, m);
        int * childGene = crossoverGene(selectedGenes, gene_length);
        childGene = mutateGene(childGene, gene_length);
        unsigned int child_time = getExeTime(childGene, gene_length, items, n, m);
        if (child_time > elite_time) {
            if (child_time <= father_time[0])
                memcpy(selectedGenes[0], childGene, gene_length * sizeof(int));
            if (child_time <= father_time[1])
                memcpy(selectedGenes[1], childGene, gene_length * sizeof(int));
        }
        elite_time = child_time < elite_time ? child_time : elite_time;
        free(childGene);
        free(selectedGenes);
        *result = elite_time;
    }
    free(population);
    pthread_exit(NULL);
}*/