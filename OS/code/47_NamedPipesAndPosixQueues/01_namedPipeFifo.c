#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_PATH     "/tmp/85_demo_fifo"
#define FIFO_BUF_SIZE 128

/*****************************************************************************
 * Name: runChild
 *
 * Description:
 *         Opens the FIFO for writing and sends a message into it. In a real
 *         use case this could be a wholly unrelated process started at any
 *         time later - here it is fork()'d only for a self-contained demo.
 *
 * Inputs:
 *         None.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void runChild(void)
{
    const char *message = "hello via named pipe";
    int fifoFd = open(FIFO_PATH, O_WRONLY);

    write(fifoFd, message, strlen(message) + 1);
    close(fifoFd);
}

int main()
{
    pid_t pid;

    // mkfifo() creates a filesystem entry of type FIFO - unlike the
    // anonymous pipe in 45_IPC/01_pipeCommunication.c (which only exists as
    // a pair of file descriptors inherited via fork(), and needs a common
    // ancestor process), a named pipe persists in the filesystem and any
    // process that knows its path can open it, related or not.
    mkfifo(FIFO_PATH, 0666);

    pid = fork();

    if (pid == 0)
    {
        runChild();
        _exit(0);
    }
    else
    {
        char buffer[FIFO_BUF_SIZE];
        int fifoFd = open(FIFO_PATH, O_RDONLY);
        ssize_t bytesRead = read(fifoFd, buffer, FIFO_BUF_SIZE);

        buffer[bytesRead > 0 ? bytesRead : 0] = '\0';
        printf("parent received: %s\n", buffer);
        close(fifoFd);

        waitpid(pid, NULL, 0);
        unlink(FIFO_PATH);
    }

    return 0;
}
