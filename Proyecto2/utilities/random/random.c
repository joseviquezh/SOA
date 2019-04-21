#include "random.h"

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

int generateRandom(int lower, int upper) 
{ 
    srand(time(NULL));
    return (rand() % (upper - lower + 1)) + lower; 
} 

int generateRandomKey() 
{ 
    return generateRandom(0, 4); 
} 

double generateExponetialDisNumber(double exp_lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / exp_lambda;
}

double generateExponetialDisNumberTest(){
    int i;
    srand((unsigned)time(NULL));
    double exp_lambda=0.05;
    for (i=0; i<20; i++)
        printf("%f\n", generateExponetialDisNumber(exp_lambda));
    return 0;
}