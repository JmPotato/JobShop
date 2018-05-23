//
// Created by JmPotato on 2018/5/10.
//

#include "IN.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int * readIuputHeader(FILE * input_txt) {
    rewind(input_txt);
    int ch, count = 0;
    ch = getc(input_txt);

    while (ch != 10 && ch != 13) {
        count++;
        ch = getc(input_txt);
    }

    int * content = (int *) malloc(count * sizeof(int));
    rewind(input_txt);
    for (int i = 0;(ch = getc(input_txt)) != 10;i++) {
        content[i] = ch;
    }

    int item_n = 0, machine_m = 0, i = 0;
    while (content[i] != 32) {
        i++;
    }
    for (int j = 0;j < i;j++) {
        item_n += (content[j] - 48) * (pow(10, i-j-1));
        if ((count-i-1) != 1)
            machine_m += (content[i+j+1] - 48) * (pow(10, count-i-2));
        else
            machine_m = content[i+1] - 48;
    }

    int * scale = (int *) malloc(2 * sizeof(int));
    scale[0] = item_n;
    scale[1] = machine_m;

    return scale;
}

ITEM * initializeIiems(FILE * input_txt, int n) {
    int ch, count = 0;
    long current_pos = ftell(input_txt);

    while ((ch = getc(input_txt)) != EOF)
        count++;

    int * content = (int *) malloc(count * sizeof(int));
    fseek(input_txt, current_pos, SEEK_SET);

    for (int i = 0;(ch = getc(input_txt)) != EOF;i++) {
        content[i] = ch;
    }

    int line = 0, comma = 0;
    ITEM * items = (ITEM *) malloc(n * sizeof(ITEM));
    for (int i = 0;i < count;i++) {
        if (content[i] != 10) {
            if (content[i] == 44)
                comma++;
        } else {
            items[line].id = line + 1;
            items[line].machine_number = comma;
            items[line].schedule = (int **) malloc(comma * sizeof(int *));
            for (int j = 0;j < comma;j++)
                items[line].schedule[j] = (int *) malloc(2 * sizeof(int));
            line++;
            comma = 0;
        }
    }

    int number = 0;
    line = 0;
    for (int i = 0;i < count;i++) {
        if (content[i] == 44) {
            if (content[i+2] == 41) {
                *(*(items[line].schedule + number) + 0) = content[i+1] - 48;
            } else {
                *(*(items[line].schedule + number) + 0) = content[i+2] - 48 + (content[i+1] - 48) * 10;
            }
            *(*(items[line].schedule + number) + 1) = content[i-1] - 48 + 1;
            number++;
        }
        if (content[i] == 10) {
            line++;
            number = 0;
        }
    }

    fseek(input_txt, current_pos, SEEK_SET);

    return items;
}

MACHINE * initializeMachines(int m, int n, ITEM * items) {
    int length = 0;
    MACHINE * machines = (MACHINE *) malloc(m * sizeof(MACHINE));

    for (int i = 0;i < m;i++) {
        machines[i].id = i + 1;
        for (int x = 0;x < n;x++) {
            for (int y = 0;y < items[x].machine_number;y++) {
                if (items[x].schedule[y][1] == machines[i].id)
                    length++;
            }
        }
        machines[i].item_list = (int *) malloc(length * sizeof(int));

        int index = 0;
        for (int x = 0;x < n;x++) {
            for (int y = 0;y < items[x].machine_number;y++) {
                if (items[x].schedule[y][1] == machines[i].id) {
                    *(machines[i].item_list + index) = items[x].id;
                    index++;
                }
            }
        }
    }

    return machines;
}