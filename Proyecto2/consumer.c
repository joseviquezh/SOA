#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#include "utilities/message/message.h"
#include "utilities/date/date.h"
#include "utilities/random/random.h"
#include "utilities/semaphore/semaphore.h"

#include "circ_buff.h"

#define STORAGE_ID "/SHARED_REGION"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be readable */
  int protection = PROT_READ | PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}

int getWaitTime (int avgWaitTime) {
    return avgWaitTime;
}

/* Main function */
int main(int argc, char *argv[])
{
    char* buffer_name;
    if(argc > 3){
      printf("There were more arguments supplied than expected\n");
      return 1;
    }
    else{
      if(strcmp("--buffer", argv[1]) == 0){
        buffer_name = argv[2];
      }
      else{
        printf("Incorrect argument %s\n", argv[1]);
        return 1;
      }
    }

    void* shmem;

    sem_t * semaphore = openSemaphore();
    if (semaphore == SEM_FAILED) perror("Opening semaphore");

    Message * message;

    int count = 0;
    int flag;

    int messagesRead = 0;
    double waitTime = 0;
    double asleepTime = 0;

    int avgWaitTime = 1;

    int consumerPid = getppid();

    int messageSize = sizeof(Message);

    int fd;
    cbuf_p cbuf;
    size_t shmem_size;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(buffer_name, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(int);
    shmem = map_file_descriptor(shmem_size, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    printf("Consumer saw file descriptor: %d\n", fd);
    printf("Consumer mapped to address: %p\n", shmem);

    cbuf = (cbuf_p) shmem;

    /* CONSUME */

    ++cbuf->consumersAlive;
    /* Wait for the semaphore */
    clock_t begin = clock();
    sem_wait(semaphore);
    clock_t end = clock();

    /* Place data from shared buffer into this process memory */
    while(cbuf->stop == false /*&& (consumerPid % 5) == message->key*/)
    {
        waitTime = waitTime += (double) (end - begin) / CLOCKS_PER_SEC;

        Message * message = calloc(1, sizeof(Message));
        int read = circ_buff_get(cbuf, message);

        printf("\n------------------- CONSUMER %i -------------------------\n", consumerPid);
        printf("MESSAGE READ \n");
        printf("Message from index: %li \n", cbuf->head); // Get index for Message in buffer
        printMessage(message);
        printf("Current consumers alive: %i \n", cbuf->consumersAlive);
        printf("--------------------------------------------------\n\n");

        ++messagesRead;
        sem_post(semaphore);

        int timeToWait = (int) generateExponetialDisNumber();
        printf("Going to sleep for %d miliseconds...\n", timeToWait);
        sleep(timeToWait);

        asleepTime = asleepTime += timeToWait;

        begin = clock();
        sem_wait(semaphore);
        end = clock();
    }
    sem_post(semaphore);
    closeSemaphore(semaphore);

    --cbuf->consumersAlive;

    printf("\n------------------- CONSUMER %i -------------------------\n", consumerPid);
    printf("FINISHED \n");
    printf("Total time blocked: %f \n", waitTime);
    printf("Total time asleep: %f\n", asleepTime);
    printf("Total messages read: %i\n", messagesRead);
    printf("------------------------------------------------------\n");

    return 0;
}
