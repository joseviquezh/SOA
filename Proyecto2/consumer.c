#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include "utilities/message/message.h"

#define BUFFER_SIZE 256
#define STORAGE_ID "/SHARED_REGION"

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

/* Main function */
int main(int argc, char *argv[])
{
    int fd;
    void* shmem;
    
    Message * message;
    
    int count = 0;
    int flag;
    
    int messagesRead = 0;
    double waitTime = 0;
    
    int avgWaitTime = 1;

    int processPid = getppid();

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(STORAGE_ID, O_RDONLY, S_IRUSR | S_IWUSR);
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

    printf("Consumer saw file descriptor: %d\n", fd);
    printf("Consumer mapped to address: %p\n", shmem);

    do {
        message = shmem + (count * sizeof(Message) + 1);
        printMessage(message);
        
        messagesRead = ++messagesRead;
        
        count = ++count;
        flag = (message->stop == 1) ? 0 : processPid % message->key;
        
        sleep(getWaitTime(avgWaitTime));
    } while (count < 5 && flag > 0);

    printf("Consumer %i ended with flag: %i\n", processPid, flag);
}