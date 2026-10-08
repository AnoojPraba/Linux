#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

// Single-threaded edge-triggered echo server (non-blocking everywhere) with a
// built-in test client thread. Key rules for EPOLLET:
//   1. all fds non-blocking;
//   2. accept in a loop until EAGAIN (one edge may cover many connections);
//   3. read in a loop until EAGAIN;
//   4. write may return EAGAIN/partial -> buffer the rest and wait for EPOLLOUT.
#define MAX_EVENTS 16
#define NCLIENTS 3

static int set_nonblock(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

static int port;                            // ephemeral port chosen by the kernel

static void *client(void *arg)
{
    long id = (long)arg;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_port = htons(port) };
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (connect(fd, (struct sockaddr *)&a, sizeof a) < 0)
        return NULL;
    char msg[32], reply[32] = {0};
    int n = snprintf(msg, sizeof msg, "hello from client %ld", id);
    write(fd, msg, n);
    ssize_t got = 0, r;
    while (got < n && (r = read(fd, reply + got, n - got)) > 0)
        got += r;
    printf("client %ld got echo: \"%s\"\n", id, reply);
    close(fd);
    return NULL;
}

int main(void)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
    struct sockaddr_in addr = { .sin_family = AF_INET,
                                .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
                                .sin_port = 0 };
    bind(lfd, (struct sockaddr *)&addr, sizeof addr);
    listen(lfd, 128);
    socklen_t len = sizeof addr;
    getsockname(lfd, (struct sockaddr *)&addr, &len);
    port = ntohs(addr.sin_port);
    set_nonblock(lfd);
    printf("server listening on 127.0.0.1:%d\n", port);

    int ep = epoll_create1(0);
    struct epoll_event ev = { .events = EPOLLIN | EPOLLET, .data.fd = lfd };
    epoll_ctl(ep, EPOLL_CTL_ADD, lfd, &ev);

    pthread_t t[NCLIENTS];
    for (long i = 0; i < NCLIENTS; i++)
        pthread_create(&t[i], NULL, client, (void *)i);

    int closed = 0;
    struct epoll_event events[MAX_EVENTS];
    while (closed < NCLIENTS)
    {
        int n = epoll_wait(ep, events, MAX_EVENTS, 5000);
        if (n <= 0)
            break;
        for (int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;
            if (fd == lfd)
            {
                for (;;)                    // drain the accept queue
                {
                    int c = accept(lfd, NULL, NULL);
                    if (c < 0)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        break;
                    }
                    set_nonblock(c);
                    setsockopt(c, IPPROTO_TCP, TCP_NODELAY, &one, sizeof one);
                    struct epoll_event ce = { .events = EPOLLIN | EPOLLET | EPOLLRDHUP,
                                              .data.fd = c };
                    epoll_ctl(ep, EPOLL_CTL_ADD, c, &ce);
                }
                continue;
            }
            int close_it = (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0;
            for (;;)                        // drain the socket
            {
                char buf[512];
                ssize_t r = read(fd, buf, sizeof buf);
                if (r > 0)
                {
                    // Echo. Real servers handle partial write / EAGAIN by
                    // queueing and arming EPOLLOUT; omitted for brevity.
                    write(fd, buf, r);
                }
                else if (r == 0) { close_it = 1; break; }
                else if (errno == EAGAIN) break;
                else if (errno == EINTR) continue;
                else { close_it = 1; break; }
            }
            if (close_it)
            {
                epoll_ctl(ep, EPOLL_CTL_DEL, fd, NULL);   // optional before close
                close(fd);
                closed++;
            }
        }
    }
    for (int i = 0; i < NCLIENTS; i++)
        pthread_join(t[i], NULL);
    printf("server closed %d connections\n", closed);
    close(ep); close(lfd);
    return 0;
}
