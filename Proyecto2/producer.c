#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    
    char* producer_message = "Hello";

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

    /* Place message in the shared buffer */
    memcpy(shmem, producer_message, sizeof(producer_message));

    printf("Producer saw file descriptor: %d\n", fd);
    printf("Producer mapped to address: %p\n", shmem);
    printf("Producer wrote: \"%s\"\n", producer_message);

    return 0;
}
