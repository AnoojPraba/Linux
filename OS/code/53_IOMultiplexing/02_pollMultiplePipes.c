#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    int pipeA[2];
    int pipeB[2];
    struct pollfd fds[2];
    char buffer[64];
    int i;

    pipe(pipeA);
    pipe(pipeB);
    write(pipeB[1], "hello from B", 12);

    // poll() takes an array of {fd, events-to-watch-for} pairs instead of
    // select()'s bitmask - no fixed-size bitmask limit (select's fd_set is
    // capped at FD_SETSIZE, typically 1024), and each pollfd reports its
    // own revents independently instead of one shared mask being mutated
    // in place.
    fds[0].fd = pipeA[0];
    fds[0].events = POLLIN;
    fds[1].fd = pipeB[0];
    fds[1].events = POLLIN;

    poll(fds, 2, -1);

    for (i = 0; i < 2; i++)
    {
        if (fds[i].revents & POLLIN)
        {
            ssize_t n = read(fds[i].fd, buffer, sizeof(buffer) - 1);

            buffer[n] = '\0';
            printf("fds[%d] (fd %d) ready: %s\n", i, fds[i].fd, buffer);
        }
    }

    // epoll (Linux-specific, not shown here) improves on both of these by
    // letting the kernel maintain the watch list across calls instead of
    // rebuilding it every time - the difference barely matters for 2
    // descriptors like this demo, but matters enormously at the scale of
    // thousands of concurrent connections (e.g. a busy TCP server), which
    // is why high-performance servers use epoll instead of select/poll.
    return 0;
}
