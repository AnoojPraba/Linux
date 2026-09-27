#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main()
{
    int pipeA[2];
    int pipeB[2];
    int epollFd;
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
    int numReady;
    int i;
    char buffer[64];

    pipe(pipeA);
    pipe(pipeB);
    write(pipeB[1], "hello from B", 12);

    // epoll_create1 makes one kernel-side object that remembers the watch
    // list between calls - epoll_ctl registers/modifies/removes individual
    // descriptors once, and epoll_wait then just asks "which of the
    // already-registered descriptors are ready" without re-describing the
    // whole set every time, unlike select()/poll() which take the full set
    // as an argument on every single call.
    epollFd = epoll_create1(0);

    event.events = EPOLLIN;
    event.data.fd = pipeA[0];
    epoll_ctl(epollFd, EPOLL_CTL_ADD, pipeA[0], &event);

    event.events = EPOLLIN;
    event.data.fd = pipeB[0];
    epoll_ctl(epollFd, EPOLL_CTL_ADD, pipeB[0], &event);

    numReady = epoll_wait(epollFd, events, MAX_EVENTS, -1);

    for (i = 0; i < numReady; i++)
    {
        ssize_t n = read(events[i].data.fd, buffer, sizeof(buffer) - 1);

        buffer[n] = '\0';
        printf("fd %d ready: %s\n", events[i].data.fd, buffer);
    }

    close(epollFd);
    return 0;
}
