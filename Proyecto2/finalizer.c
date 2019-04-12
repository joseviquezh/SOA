#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256
#define STORAGE_ID "/SHARED_REGION"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be readable and writable: */
  int protection = PROT_READ | PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}

/* Main function */
int main(int argc, char *argv[])
{
    int ret;
    int fd;
    void* shmem;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    printf("Shared buffer to be cleaned: %d\n", fd);

    /* Map file descriptor to an address region */
    shmem = map_file_descriptor(BUFFER_SIZE, fd);

    if (shmem == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    /* mmap cleanup */
    ret = munmap(shmem, BUFFER_SIZE);
    if (ret == -1)
    {
        perror("munmap");
        return ret;
    }
    
    /* shm_open cleanup */
    fd = shm_unlink(STORAGE_ID);
    if (fd == -1)
    {
        perror("unlink");
        return fd;
    }

    return 0;
}
