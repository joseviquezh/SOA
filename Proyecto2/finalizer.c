#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#include "utilities/semaphore/semaphore.h"

#include "circ_buff.h"

#define STORAGE_ID "/SHARED_REGION"

sem_t* semaphore;
void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be readable and writable: */
  int protection = PROT_READ | PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}


void printIntrucctions(){
  printf("Expected %d mandatory and %d optional parameters\n",2,0);
  printf("Mandatory: Add \"--time\" <Seconds>: Waiting time before turn off all processors\n");
  printf("Mandatory: Add \"--buffer\" <Buffer Name>\n");
}

/* Main function */
int main(int argc, char *argv[])
{
    int quantum;
    char* buffer_name;
    if(argc > 5){
      printf("There were more arguments supplied than expected\n");
      return 1;
    }else if(argc<5){
      printIntrucctions();
      return 1;
    }
    else{
      if(strcmp("--time", argv[1]) == 0){
        quantum = atoi(argv[2]);
      }
      else{
        printf("Incorrect argument %s\n", argv[1]);
        return 1;
      }
      if(strcmp("--buffer", argv[3]) == 0){
        buffer_name = argv[4];
      }
      else{
        printf("Incorrect argument %s\n", argv[3]);
        return 1;
      }
    }

    int ret;
    int fd;
    void* shmem;
    cbuf_p cbuf;
    size_t shmem_size;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(buffer_name, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    printf("Shared buffer to be cleaned: %d\n", fd);

    /* Map file descriptor to an address region */
    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(int);
    shmem = map_file_descriptor(shmem_size, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    cbuf = (cbuf_p) shmem;

    semaphore = openSemaphore();
    if (semaphore == SEM_FAILED) perror("Opening semaphore");

    printf("Letting the processes to work for %d seconds\n", quantum);
    sleep(quantum);
    printf("Time is up!\n");

    sem_wait(semaphore);

    /* Notify producers and consumers to stop*/
    cbuf->stop = true;

    sem_post(semaphore);

    printf("Waiting for all processes to end thei execution\n");

    sem_wait(semaphore);
    while(cbuf->consumersAlive > 0 || cbuf->producersAlive >0){
      sem_post(semaphore);
      sleep(1);
      sem_wait(semaphore);
    }

    printf("Cleaning shared memory\n");
    /* mmap cleanup */
    ret = munmap(shmem, BUFFER_SIZE);
    if (ret == -1)
    {
        perror("munmap");
        return ret;
    }

    /* shm_open cleanup */
    fd = shm_unlink(buffer_name);
    if (fd == -1)
    {
        perror("unlink");
        return fd;
    }

    closeSemaphore(semaphore);

    unlinkSemaphore();

    printf("The cleaning was successful\n");
    return 0;
}
