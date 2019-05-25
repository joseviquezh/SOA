// System includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// Custom includes
#include "task.h"
#include "queue-item.h"
#include "test/test.h"
#include "scheduler/scheduler.h"

// Main
int main(int argc, char *argv[])
{
    /*int tasks_length = 6;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    for (int i = 0; i < tasks_length; i++) {
        tasks[i] = (Task) { i + 1, (i + 2) * 2 };;
    }*/

    /*int tasks_length = 3;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    tasks[0] = (Task) { 1, 1, 2 };
    tasks[1] = (Task) { 2, 1, 3 };
    tasks[2] = (Task) { 3, 1, 6 };*/

    int tasks_length = 2;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    tasks[0] = (Task) { 1, 3, 6 };
    tasks[1] = (Task) { 2, 4, 9 };

    /*int tasks_length = 3;
    Task * tasks = calloc(tasks_length, sizeof(Task));

    tasks[0] = (Task) { 1, 1, 6 };
    tasks[1] = (Task) { 2, 2, 9 };
    tasks[2] = (Task) { 3, 6, 18 };*/

    printf("\n==========\n");
    printf("i | c | p\n----------\n");
    for (int i = 0; i < tasks_length; i++) {
        Task task = tasks[i];
        printf("%i | %i | %i\n", task.id, task.period, task.computation_time);
    }
    printf("==========\n\n\n");

    int test_result = TestLLF(tasks, tasks_length);
    printf("test_result = %i\n\n\n", test_result);

    InitScheduler(tasks_length, tasks, LLF);
    RunScheduling();

    int history_size = GetHistorySize ();
    int has_deadlines = HasMissedDeadlines ();
    printf ("history length = %i\n", history_size);

    QueueItem * history = GetHistory ();
    for (int i = 0; i < history_size; i++) {
        printf("\n============= Time elapsed %i ==============\n", i);

        if (!history[i].null) printf("Running task: %i in elapsed_time = %i\n", history[i].task.id, i);
        else printf("Free period in elapsed_time = %i\n", i);

        printf("===========================================\n\n\n");
    }

    return 0;
}
