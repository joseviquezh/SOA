// File header
#include "scheduler.h"

// System includes
#include <stdlib.h>
#include <stdio.h>

// Custom includes
#include "../task.h"
#include "../queue-item.h"

// Properties
QueueItem * ready_queue;
Task * tasks;

QueueItem history[2000];
int history_size;

int elapsed_time;
int current_items_in_queue;
int total_tasks;

int end_flag;
int deadline_missed_flag;

int algorithm;

int mcm;

// -----------
// Helpers
// ----------

void RMSortReadyQueue (QueueItem * p_ready_queue, int p_size) {
    // Bubble sort because of the quantity of tasks.
    for (int i = 0; i < p_size; i++) {
        QueueItem item_i = p_ready_queue[i];
        for (int j = i + 1; j < p_size; j++) {
            QueueItem item_j = p_ready_queue[j];

            if (item_j.priority < item_i.priority) {
                p_ready_queue[i] = item_j;
                p_ready_queue[j] = item_i;

                item_i = item_j;
            }
        }
    }
}

void EDFSortReadyQueue (QueueItem * p_ready_queue, int p_size) {

    for (int i = 0; i < p_size; i++) {
        p_ready_queue[i].priority -= 1;
    }

    RMSortReadyQueue (p_ready_queue, p_size);
}

void LLFSortReadyQueue (QueueItem * p_ready_queue, int p_size) {
    int a = elapsed_time;
    for (int i = 0; i < p_size; i++) {
        int x = 0, d = 0, c = 0, period = 0;
        d = period = p_ready_queue[i].task.period;
        while(d < a){
            d += period;
        }
        c = p_ready_queue[i].task.computation_time;
        x = d - a - c;

        p_ready_queue[i].priority = x;
    }

    RMSortReadyQueue (p_ready_queue, p_size);
}

void SortReadyQueue () {
    switch (algorithm) {
        case RM:
            RMSortReadyQueue(ready_queue, current_items_in_queue);
            break;
        case EDF:
            EDFSortReadyQueue(ready_queue, current_items_in_queue);
        case LLF:
            LLFSortReadyQueue(ready_queue, current_items_in_queue);
        default:
            break;
    }
}


// -----------
// File definition methods.
// ----------

void InitScheduler (int p_tasks_length, Task * p_tasks, int p_algorithm) {
    // Queue initialization.
    ready_queue = calloc(2 * p_tasks_length, sizeof(QueueItem)); //; in case deadline of a task is missed

    for (int i = 0; i < p_tasks_length; i++) {
        ready_queue[i] = (QueueItem) { p_tasks[i], p_tasks[i].period, p_tasks[i].computation_time, 0, 0 };
    }

    // Properties init
    algorithm = p_algorithm;
    tasks = p_tasks;
    current_items_in_queue = p_tasks_length;
    elapsed_time = 0;
    total_tasks = p_tasks_length;
    history_size = 0;

    // Flags initialization.
    end_flag = 0;
    deadline_missed_flag = 0;

    SortReadyQueue();
}

void RemoveHeadFromQueue () {
    for (int i = 1; i < current_items_in_queue; i++) {
        ready_queue[i - 1] = ready_queue[i];
    }
    current_items_in_queue -= 1;
}

void CheckForDeadlines () {
    for (int i = 0; i < current_items_in_queue; i++) {
        Task task_i = ready_queue[i].task;
        for (int j = i + 1; j < current_items_in_queue; j++) {
            Task task_j = ready_queue[j].task;
            if (task_i.id == task_j.id && i != j) deadline_missed_flag += 1;
        }
    }
}

int CalculateGcd(int a, int b){
  if(b == 0){
    return a;
  }
  return CalculateGcd(b, a % b);
}

int CalculateMcm(int a, int b){
  int gcd = CalculateGcd(a, b);
  return (a * b)/gcd;
}

void AddTasksToQueue () {
    int total_added = 0;
    for (int i = 0; i < total_tasks; i++) {
        if (elapsed_time % tasks[i].period == 0) {
            ready_queue[current_items_in_queue] = (QueueItem) { tasks[i], tasks[i].period, tasks[i].computation_time, elapsed_time, 0 };
            current_items_in_queue += 1;
            total_added += 1;
        }
    }

    // if (total_added == total_tasks) end_flag = 1;
    if (elapsed_time ==  mcm) end_flag = 1;

    CheckForDeadlines ();
}

void ApplyAdvancedToQueue () {

    if (current_items_in_queue > 0) {
        history[history_size] = ready_queue[0];
    } else {
        history[history_size] = (QueueItem) { .null = 1 };
    }

    history_size += 1;

    if (current_items_in_queue > 0){
        ready_queue[0].remaining_periods -= 1;
        if (ready_queue[0].remaining_periods == 0) RemoveHeadFromQueue();
    }

    AddTasksToQueue ();
}

int HasMissedDeadlines () {
    return deadline_missed_flag;
}

QueueItem * GetHistory () {
    static QueueItem copy[2000];
    for (int i = 0; i < history_size; i++) {
        copy[i] = history[i];
    }
    return copy;
}

int GetHistorySize () {
    return history_size;
}

int GetMcm () {
    return mcm;
}

int RunScheduling () {
    // Calculate the MCM between the periods of all the tasks
    mcm = 1;
    for (int i = 0; i < total_tasks; i++) {
        mcm = CalculateMcm(tasks[i].period, mcm);
    }

    printf("The mcm between the periods is %d", mcm);

    while (end_flag == 0 && deadline_missed_flag == 0) {

        elapsed_time += 1;
        ApplyAdvancedToQueue();
        SortReadyQueue();
    }

    printf("\nend_flag = %i, deadline_flag = %i\n\n", end_flag, deadline_missed_flag);

    return 0;
}
