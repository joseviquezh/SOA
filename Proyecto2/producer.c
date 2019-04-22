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

void printIntrucctions(){
  printf("Expected %d mandatory and %d optional parameters\n",1,1);
  printf("Mandatory: Add \"--buffer\" <Buffer Name>\n");
  printf("Optional: Add \"--lambda\" <Number>:  Lambda exponential distribution parameter *default value 1\n");
}


/* Main function */
int main(int argc, char *argv[])
{
    char* buffer_name;
    double exp_lambda=1;

    if(argc != 5 && argc != 3){
      printf("Incorrect ammount of arguments were supplied\n");
      printIntrucctions();
      return 1;
    }
    else{
      for(int i = 1; i < argc; ++i){
        if(strcmp("--lambda", argv[i]) == 0){
          exp_lambda = atof(argv[++i]);
        }
        else if(strcmp("--buffer", argv[i]) == 0){
          buffer_name = argv[++i];
        }
        else{
          printf("Unrecognized argument %s\n", argv[i]);
          printIntrucctions();
          return 1;
        }
      }
    }

    void* shmem;

    sem_t * semaphore = openSemaphore();
    if (semaphore == SEM_FAILED){
      perror("Opening semaphore");
      return 1;
    }

    Message * message;

    int count = 0;
    int flag;

    int messagesPost = 0;
    double waitTime = 0;
    double asleepTime = 0;

    int avgWaitTime = 1;

    pid_t producerPid = getpid();

    int messageSize = sizeof(Message);

    int fd;
    cbuf_p cbuf;
    size_t shmem_size;

    /* Get shared memory file descriptor on the region */
    fd = shm_open(buffer_name, O_RDWR , S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(Message);
    shmem = map_file_descriptor(shmem_size, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    printf("Producer saw file descriptor: %d\n", fd);
    printf("Producer mapped to address: %p\n", shmem);


    cbuf = (cbuf_p) shmem;

    /* PRODUCE */

    ++cbuf->producersAlive;

    /* Wait for the semaphore*/
    clock_t begin = clock();
    sem_wait(semaphore);
    clock_t end = clock();
    srand((unsigned)time(NULL));
    ++cbuf->totalProducers;
    /* Place messages in the buffer*/
    while(cbuf->stop == false)
    {
        waitTime = waitTime += (double) (end - begin) / CLOCKS_PER_SEC;

        Message * message = calloc(1, sizeof(Message));
        int randomKey = generateRandomKey();
        *message = (Message) { producerPid, randomKey, 0, getCurrentDateTime() };

        printf("\n\n -------- PRODUCER %i ---------\n", producerPid);
        printf("MESSAGE WRITE\n");
        printf("Message from index: %li \n", cbuf->head); // Get index for Message in buffer
        printMessage(message);
        printf("Current producers alive: %i \n", cbuf->producersAlive);
        printf("-----------------------------------\n");
        circ_buff_set(cbuf, *message);
        ++cbuf->totalMessagesCreated;
        sem_post(semaphore);
        ++messagesPost;

        int  timeToWait =(int) (generateExponetialDisNumber(exp_lambda)*1000000.0);
        printf("Going to sleep for %d miliseconds...\n",(int)(timeToWait/1000));
        usleep(timeToWait);

        asleepTime = asleepTime += timeToWait;

        begin = clock();
        sem_wait(semaphore);
        end = clock();
    }
    sem_post(semaphore);
    closeSemaphore(semaphore);

    --cbuf->producersAlive;

    printf("\n------------------- PRODUCER %i -------------------------\n", producerPid);
    printf("FINISHED \n");
    printf("Total time blocked: %f \n", waitTime);
    printf("Total time asleep: %f\n", asleepTime);
    printf("Total messages posted: %i\n", messagesPost);
    printf("------------------------------------------------------\n");

    return 0;
}
