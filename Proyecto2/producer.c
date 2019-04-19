#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>

#include "utilities/message/message.h"
#include "utilities/date/date.h"
#include "utilities/random/random.h"
#include "utilities/semaphore/semaphore.h"

#define BUFFER_SIZE 256
#define STORAGE_ID "/SHARED_REGION"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be writable: */
  int protection = PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}

/* Main function */
int main(int argc, char *argv[])
{
    int fd;
    void* shmem;
    
    int processPid = getppid();
    sem_t * semaphore = openSemaphore();
    
    int count = 0;
    int flag;

    /* Get shared memory file descriptor on the region */
    fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    /* Map file descriptor to an address region */
    shmem = map_file_descriptor(BUFFER_SIZE, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    printf("Producer saw file descriptor: %d\n", fd);
    printf("Producer mapped to address: %p\n", shmem);

    do {
        sem_wait(semaphore);

        Message * message = calloc(1, sizeof(Message));
        int randomKey = generateRandomKey();
        *message = (Message) { processPid, randomKey, 0, getCurrentDateTime() };
        
        /* Place message in the shared buffer */
        memcpy(shmem + (count * sizeof(Message)), message, sizeof(Message));

        count = ++count;

        sem_post(semaphore);

        sleep(1);
        //free(message);
    } while (count < 20);

    printf("Producer %i ended \n", processPid);

    return 0;
}
