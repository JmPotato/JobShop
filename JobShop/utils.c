//
// Created by JmPotato on 2018/5/14.
//
#include "utils.h"

#include <time.h>
#include <stdlib.h>

int randomInterger(int a, int b) {
    srand((unsigned)time(NULL));

    int interger = (rand() % (b-a+1)) + a;

    return interger;
}