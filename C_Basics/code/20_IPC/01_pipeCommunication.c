#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PIPE_BUF_SIZE 128

int main()
{
    int fd[2];
    pid_t pid;
    char buffer[PIPE_BUF_SIZE];

    // fd[0] is the read end, fd[1] is the write end - a pipe is one-way,
    // so a fork'd child sharing these descriptors can write while the
    // parent reads (or vice versa).
    pipe(fd);

    pid = fork();

    if (pid == 0)
    {
        const char *message = "hello from child";

        close(fd[0]);
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        read(fd[0], buffer, PIPE_BUF_SIZE);
        close(fd[0]);
        printf("parent received: %s\n", buffer);
        waitpid(pid, NULL, 0);
    }

    return 0;
}
