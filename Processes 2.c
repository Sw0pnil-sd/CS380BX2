#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SHM_NAME "/collatz_shm"
#define SHM_SIZE 1024

int main(int argc, char *argv[])
{
    int shm_fd;
    int *shm_ptr;
    pid_t pid;
    int n, index = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <starting_number>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "Starting number must be positive\n");
        return 1;
    }

    /* Create shared memory object */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    /* Set size of shared memory */
    ftruncate(shm_fd, SHM_SIZE);

    /* Map shared memory */
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    /* Child process: generate Collatz sequence */
    if (pid == 0)
    {
        while (n != 1)
        {
            shm_ptr[index++] = n;

            if (n % 2 == 0)
                n = n / 2;
            else
                n = 3 * n + 1;
        }
        shm_ptr[index++] = 1;
        shm_ptr[index] = -1; // Sentinel value

        exit(0);
    }

    /* Parent process: wait and print */
    else
    {
        wait(NULL);

        printf("Collatz sequence: ");
        for (int i = 0; shm_ptr[i] != -1; i++)
        {
            printf("%d ", shm_ptr[i]);
        }
        printf("\n");

        /* Cleanup */
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
