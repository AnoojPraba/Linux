#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
    int pipeA[2];
    int pipeB[2];
    fd_set readFds;
    int maxFd;
    char buffer[64];

    pipe(pipeA);
    pipe(pipeB);

    // Only write to pipeB - this simulates one of several inputs becoming
    // ready while the others stay quiet, which is exactly the situation
    // select()/poll() exist to handle: waiting on many file descriptors at
    // once instead of blocking on read() for just one of them.
    write(pipeB[1], "hello from B", 12);

    // select() takes a bitmask of descriptors to watch (built via
    // FD_ZERO/FD_SET) and blocks until at least one becomes readable -
    // FD_ISSET afterward checks which one(s) actually triggered it.
    FD_ZERO(&readFds);
    FD_SET(pipeA[0], &readFds);
    FD_SET(pipeB[0], &readFds);
    maxFd = (pipeA[0] > pipeB[0]) ? pipeA[0] : pipeB[0];

    select(maxFd + 1, &readFds, NULL, NULL, NULL);

    if (FD_ISSET(pipeA[0], &readFds))
    {
        ssize_t n = read(pipeA[0], buffer, sizeof(buffer) - 1);

        buffer[n] = '\0';
        printf("pipeA ready: %s\n", buffer);
    }
    if (FD_ISSET(pipeB[0], &readFds))
    {
        ssize_t n = read(pipeB[0], buffer, sizeof(buffer) - 1);

        buffer[n] = '\0';
        printf("pipeB ready: %s\n", buffer);
    }

    return 0;
}
