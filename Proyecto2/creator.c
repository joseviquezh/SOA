#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>

#include "utilities/semaphore/semaphore.h"

#include "circ_buff.h"

#define STORAGE_ID "/SHARED_REGION"
#define CONSUMERS_ALIVE "/CONSUMERS_ALIVE"
#define PRODUCERS_ALIVE "/PRODUCERS_ALIVE"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be writable: */
  int protection = PROT_READ | PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}
void printIntrucctions(){
  printf("Expected %d mandatory and %d optional parameters\n",1,0);
  printf("Mandatory: Add \"--buffer\" <Buffer Name>\n");
}

/* Main function */
int main(int argc, char *argv[])
{
    char* buffer_name;
    if(argc > 3){
      printf("There were more arguments supplied than expected\n");
      printIntrucctions();
      return 1;
    }else if(argc<3){
      printIntrucctions();
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
    cbuf_p cbuf;
    size_t shmem_size;
    sem_t * semaphore;

    /* Get shared memory file descriptor on the region*/
    int fd = shm_open(buffer_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(int);

    /* Extend shared memory object as by default it's initialized with size 0 */
    int ret = ftruncate(fd, BUFFER_SIZE);
    if (ret == -1)
    {
        perror("ftruncate");
        return ret;
    }
    /*
    semaphore = openSemaphore();
    unlinkSemaphore();
    closeSemaphore(semaphore);
    */
    semaphore = createSemaphore();
    if (semaphore == SEM_FAILED) perror("Creating semaphore");
    closeSemaphore(semaphore);

    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(int);

    /* Map file descriptor to an address region */
    shmem = map_file_descriptor(shmem_size, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    cbuf = (cbuf_p) shmem;

    /* Initialize circular buffer */
    circ_buff_init(cbuf, BUFFER_SIZE);

    printf("Shared buffer created: %d\n", fd);
    return 0;
}
