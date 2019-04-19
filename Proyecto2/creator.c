#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "circ_buff.h"

#define STORAGE_ID "/SHARED_REGION"

void* map_file_descriptor(size_t size, int fd) {
  /* The memory buffer will be writable: */
  int protection = PROT_READ | PROT_WRITE;

  /* Only this process and its children will be able to use it */
  int visibility = MAP_SHARED;

  return mmap(NULL, size, protection, visibility, fd, 0);
}

/* Main function */
int main(int argc, char *argv[])
{
    int ret;
    int fd;
    void* shmem;
    cbuf_p cbuf;
    size_t shmem_size;

    /* Get shared memory file descriptor on the region*/
    fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    shmem_size = sizeof(circ_buff) + BUFFER_SIZE * sizeof(int);

    /* Extend shared memory object as by default it's initialized with size 0 */
    ret = ftruncate(fd, shmem_size);
    if (ret == -1)
    {
        perror("ftruncate");
        return ret;
    }

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
