// File header
#include "semaphore.h"

// System includes
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>

#define SEMAPHORE_NAME "producer_consumer"

sem_t * createSemaphore () {
    return sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, 1);
}

void unlinkSemaphore () {
    int result = sem_unlink(SEMAPHORE_NAME);
    if (result == -1){
      perror("Unlinking semaphore");
    }
}

void closeSemaphore (sem_t* sem) {
    int result = sem_close(sem);
    if (result == -1){
      perror("Closing semaphore");
    }
}

sem_t * openSemaphore () {
    return sem_open(SEMAPHORE_NAME, O_RDWR);
}
