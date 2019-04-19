#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#include "utilities/semaphore/semaphore.h"

#define BUFFER_SIZE 256
#define STORAGE_ID "/SHARED_REGION"

/* Main function */
int main(int argc, char *argv[])
{
    int ret;
    int fd;

    /* get shared memory file descriptor on the region*/
    fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return fd;
    }

    /* Extend shared memory object as by default it's initialized with size 0 */
    ret = ftruncate(fd, BUFFER_SIZE);
    if (ret == -1)
    {
        perror("ftruncate");
        return ret;
    }

    sem_close(createSemaphore());

    printf("Shared buffer created: %d\n", fd);
    return 0;
}
