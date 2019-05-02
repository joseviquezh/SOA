#include <math.h>
#include <stdio.h>

#include "test.h"
#include "../task.h"

// For presentation & helpers
double CalculateMiu (Task * tasks, int tasks_length) {
    double result = 0;
    for (int i = 0; i < tasks_length; i++) {
        Task task = tasks[i];
        result += (double) task.computation_time / (double) task.period; 
    }

    return result;
}

double CalculateMaxUtilizationRM (int tasks_length) {
    return (double)tasks_length * (pow(2, 1 / (double)tasks_length) - 1);
}

double CalculateMaxUtilizationEDF () {
    return 1;
}

// Main tests   
int TestRM (Task * tasks, int tasks_length) {
    double miu = CalculateMiu (tasks, tasks_length);
    double max_utilization = (double)tasks_length * (pow(2, 1 / (double)tasks_length) - 1);

    printf("miu = %lf, max_utilization = %lf\n", miu, max_utilization);

    return miu <= max_utilization;
}

int TestEDF (Task * tasks, int tasks_length) {
    double miu = CalculateMiu (tasks, tasks_length);
    printf("miu = %lf, max_utilization = 1\n", miu);
    return miu <= CalculateMaxUtilizationEDF();
}

