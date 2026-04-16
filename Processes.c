#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

int main()
{
    int fd[2]; // fd[0] = read end, fd[1] = write end
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Creating pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Creating child process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    /* Parent process */
    if (pid > 0)
    {
        close(fd[0]); // Closing unused read end

        printf("Enter a string: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        write(fd[1], buffer, strlen(buffer) + 1);
        close(fd[1]); // Closing write end
    }
    /* Child process */
    else
    {
        close(fd[1]); // Closing unused write end

        read(fd[0], buffer, BUFFER_SIZE);

        // Invert case
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (islower(buffer[i]))
                buffer[i] = toupper(buffer[i]);
            else if (isupper(buffer[i]))
                buffer[i] = tolower(buffer[i]);
        }

        printf("Modified string: %s", buffer);
        close(fd[0]); // Closing read end
    }

    return 0;
}
