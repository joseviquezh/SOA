// File header
#include "RM.h"

// System includes
#include <stdlib.h>
#include <stdio.h>

// Custom includes
#include "../task.h"
#include "../queue-item.h"

// Properties
QueueItem * RM_ready_queue;
Task * RM_tasks;

Task RM_history[2000];
int RM_history_size;

int RM_elapsed_time;
int RM_current_items_in_queue;
int RM_total_tasks;

int RM_end_flag;
int RM_deadline_missed_flag;
int RM_one_element_popped;

void RM_SortReadyQueue () {
    // Bubble sort because of the quantity of tasks.
    for (int i = 0; i < RM_total_tasks; i++) {
        QueueItem item_i = RM_ready_queue[i];
        for (int j = i + 1; j < RM_total_tasks; j++) {
            QueueItem item_j = RM_ready_queue[j];
            if (item_j.priority < item_i.priority) {
                RM_ready_queue[i] = item_j;
                RM_ready_queue[j] = item_i;

                item_i = item_j;
            }
        }
    }
}

void RM_Init (int p_tasks_length, Task * p_tasks) {
    // Queue initialization.
    RM_ready_queue = calloc(2 * p_tasks_length, sizeof(QueueItem)); //; in case deadline of a task is missed
    
    for (int i = 0; i < p_tasks_length; i++) {
        RM_ready_queue[i] = (QueueItem) { p_tasks[i], p_tasks[i].period, p_tasks[i].computation_time };
    }
    
    for (int i = 0; i < p_tasks_length; i++) {
        printf("priority = %i, remaining_periods = %i\n", RM_ready_queue[i].priority, RM_ready_queue[i].remaining_periods);
    }

    // Properties init
    RM_tasks = p_tasks;
    RM_current_items_in_queue = p_tasks_length;
    RM_elapsed_time = 0;
    RM_total_tasks = p_tasks_length;
    RM_history_size = 0;

    // Flags initialization.
    RM_end_flag = 0;
    RM_deadline_missed_flag = 0;
    RM_one_element_popped = 0;

    RM_SortReadyQueue();
    printf("\n\nSorted...\n");
    for (int i = 0; i < p_tasks_length; i++) {
        printf("priority = %i, remaining_periods = %i\n", RM_ready_queue[i].priority, RM_ready_queue[i].remaining_periods);
    }
}

void RM_RemoveHeadFromQueue () {
    RM_one_element_popped = 1;
    for (int i = 1; i < RM_current_items_in_queue; i++) {
        RM_ready_queue[i - 1] = RM_ready_queue[i]; 
    }
    RM_current_items_in_queue -= 1;
}

void RM_CheckForDeadlines () {
    for (int i = 0; i < RM_current_items_in_queue; i++) {
        Task task_i = RM_ready_queue[i].task;
        for (int j = 0; j < RM_current_items_in_queue; j++) {
            Task task_j = RM_ready_queue[j].task;
            if (task_i.id == task_j.id && i != j) RM_deadline_missed_flag += 1;
        }
    }

    if (RM_deadline_missed_flag > 0) RM_deadline_missed_flag = RM_deadline_missed_flag / 2; 
}

void RM_AddTasksToQueue () {
    int total_added = 0;
    for (int i = 0; i < RM_total_tasks; i++) {
        if (RM_elapsed_time % RM_tasks[i].period == 0) {
            RM_ready_queue[RM_current_items_in_queue] = (QueueItem) { RM_tasks[i], RM_tasks[i].period, RM_tasks[i].computation_time };
            RM_current_items_in_queue += 1;
            total_added += 1;
        }
    }

    if (total_added == RM_total_tasks) RM_end_flag = 1;
    
    RM_CheckForDeadlines ();
}

void RM_ApplyAdvancedToQueue () {

    RM_history[RM_history_size] = RM_ready_queue[0].task;
    RM_history_size += 1;

    if (RM_current_items_in_queue > 0){ 
        RM_ready_queue[0].remaining_periods -= 1;
        if (RM_ready_queue[0].remaining_periods == 0) RM_RemoveHeadFromQueue();
    }

    RM_AddTasksToQueue();
}

int RM_HasMissedDeadlines () {
    return RM_deadline_missed_flag;
}

Task * RM_GetHistory () {
    return RM_history;
}

int RM_GetHistorySize () {
    return RM_history_size;
}

int RM_Run () {
    while (RM_end_flag == 0 &&  RM_deadline_missed_flag == 0) {
        
        RM_elapsed_time += 1;
        RM_ApplyAdvancedToQueue();
        RM_SortReadyQueue();
        
        printf("\n============= Time elapsed ==============\n");
        
        printf("Running task: %i in elapsed_time = %i\n", RM_history[RM_history_size - 1].id, RM_elapsed_time);

        printf("--------\nReady Queue\n--------\n");
        for (int i = 0; i < RM_current_items_in_queue; i++) {
            printf("priority = %i, remaining_periods = %i\n", RM_ready_queue[i].priority, RM_ready_queue[i].remaining_periods);
        }
        printf("--------\n");
        printf("===========================================\n\n\n");
    }

    printf("\nend_flag = %i, deadline_flag = %i\n\n", RM_end_flag, RM_deadline_missed_flag);
}

int RM_GetCurrentTaskIndex () {
    if (RM_current_items_in_queue) return RM_ready_queue[0].task.id;
    return 0;
};
