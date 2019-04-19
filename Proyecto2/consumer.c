#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

#include "utilities/message/message.h"
#include "utilities/semaphore/semaphore.h"

#define BUFFER_SIZE 256
#define STORAGE_ID "/SHARED_REGION"
#define CONSUMERS_ALIVE "/CONSUMERS_ALIVE"
#define PRODUCERS_ALIVE "/PRODUCERS_ALIVE"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be readable */
  int protection = PROT_READ;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}

int getWaitTime (int avgWaitTime) {
    return avgWaitTime;
}

void * openSharedRegion (char * name, int size) {
    int fd;
    void* shreg;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(STORAGE_ID, O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
    }

    /* Map file descriptor to an address region */
    return map_file_descriptor(BUFFER_SIZE, fd);
}

/* Main function */
int main(int argc, char *argv[])
{
    void* shmem;
    int * consumersAlive;
    
    sem_t * semaphore = openSemaphore();
    if (semaphore == SEM_FAILED) printf("ERROR | Couldn't open the semaphore\n");
    
    Message * message;
    
    int count = 0;
    int flag;
    
    int messagesRead = 0;
    double waitTime = 0;
    double asleepTime = 0;
    
    int avgWaitTime = 1;

    int processPid = getppid();

    int messageSize = sizeof(Message);

    shmem = openSharedRegion(STORAGE_ID, BUFFER_SIZE);
    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    consumersAlive = openSharedRegion(CONSUMERS_ALIVE, sizeof(int));
    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    consumersAlive = consumersAlive += 1;

    //printf("Consumer saw file descriptor: %d\n", fd);
    printf("Consumer mapped to address: %p\n", shmem);

    do {

        clock_t begin = clock();
        sem_wait(semaphore);
        clock_t end = clock();

        waitTime = waitTime += (double) (end - begin) / CLOCKS_PER_SEC;
        
        message = shmem + (count * messageSize);
        
        printf("\n--------------------------------------------------\n");
        printf("Consumer ID: %i\n", processPid);
        printf("Message from index: %i \n", count);
        printMessage(message);
        printf("Current consumers alive: %i \n", *consumersAlive);
        printf("--------------------------------------------------\n\n");

        messagesRead = ++messagesRead;

        count = ++count;
        flag = (message->stop == 1) ? 0 : ( (processPid % 5) == message->key ? 0 : 1 );

        sem_post(semaphore);
        
        int timeToWait = getWaitTime(avgWaitTime);
        sleep(timeToWait);
        
        asleepTime = asleepTime += timeToWait;

    } while (count < 20 && flag > 0);

    consumersAlive = consumersAlive -= 1;
    
    printf("------------------------------------------------------\n");
    printf("Consumer %i ended \n", processPid);
    printf("Total time blocked: %f \n", waitTime);
    printf("Total time asleep: %f\n", asleepTime);
    printf("Total messages read: %i\n", messagesRead);
    printf("------------------------------------------------------\n");

}