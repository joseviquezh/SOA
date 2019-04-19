// File header
#include "semaphore.h"

// System includes
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>

#define SEMAPHORE_NAME "producer_consumer"

sem_t * createSemaphore () {
    return sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 0);
}

void removeSemaphore () {
    int result = sem_unlink(SEMAPHORE_NAME);
    if (result == -1) printf("Error occurred while unlinking the semaphore\n");
}

sem_t * openSemaphore () {
    return sem_open(SEMAPHORE_NAME, 0);
}