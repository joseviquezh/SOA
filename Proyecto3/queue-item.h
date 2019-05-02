#include "task.h"

#ifndef QUEUE_ITEM_H_
#define QUEUE_ITEM_H_

typedef struct {
    Task task;
    int priority;
    int remaining_periods;
} QueueItem;

#endif //QUEUE_ITEM_H_