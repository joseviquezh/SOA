#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

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

int createSharedRegion (char * name, int size) {

    /* get shared memory file descriptor on the region*/
    int fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    /* Extend shared memory object as by default it's initialized with size 0 */
    int ret = ftruncate(fd, size);
    if (ret == -1)
    {
        perror("ftruncate");
        return ret;
    }

    return fd;
}

/* Main function */
int main(int argc, char *argv[])
{
    int fd;

    fd = createSharedRegion(STORAGE_ID, BUFFER_SIZE);
    createSharedRegion(CONSUMERS_ALIVE, sizeof(int));

    // ---- Init counters 0
    int fdc;
    int* shreg;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(CONSUMERS_ALIVE, O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
    }

    /* Map file descriptor to an address region */
    shreg = map_file_descriptor(sizeof(int), fd);
    shreg = 0;
    // -----

    sem_close(createSemaphore());

    printf("Shared buffer created: %d\n", fd);
    return 0;
}
