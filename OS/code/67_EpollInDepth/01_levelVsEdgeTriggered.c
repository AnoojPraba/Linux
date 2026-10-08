#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

// Level-triggered (default): epoll_wait reports an fd as long as the
// condition holds (data still unread). Edge-triggered (EPOLLET): it reports
// only on a state CHANGE (new data arrived) - you must drain to EAGAIN or you
// will never be told about the leftover bytes.
static int count_ready(int ep)
{
    struct epoll_event ev[4];
    return epoll_wait(ep, ev, 4, 0);          // timeout 0 = poll, don't block
}

static void test(const char *name, unsigned flags)
{
    int p[2];
    pipe(p);
    fcntl(p[0], F_SETFL, O_NONBLOCK);
    int ep = epoll_create1(0);
    struct epoll_event e = { .events = EPOLLIN | flags, .data.fd = p[0] };
    epoll_ctl(ep, EPOLL_CTL_ADD, p[0], &e);

    write(p[1], "0123456789", 10);
    printf("%s\n", name);
    printf("  after write(10):          ready=%d\n", count_ready(ep));

    char buf[4];
    read(p[0], buf, 4);                       // consume only part
    printf("  after partial read(4):    ready=%d  %s\n", count_ready(ep),
           flags & EPOLLET ? "(edge: 6 bytes left but NO new event - stall risk)"
                           : "(level: still readable, reported again)");

    write(p[1], "X", 1);
    printf("  after another write(1):   ready=%d  (a new edge)\n", count_ready(ep));

    while (read(p[0], buf, sizeof buf) > 0)   // drain to EAGAIN
        ;
    printf("  drained, errno==EAGAIN:   %s\n", errno == EAGAIN ? "yes" : "no");
    close(ep); close(p[0]); close(p[1]);
}

int main(void)
{
    test("LEVEL-TRIGGERED", 0);
    test("EDGE-TRIGGERED (EPOLLET)", EPOLLET);

    // EPOLLONESHOT: after one event the fd is disarmed until re-armed with
    // EPOLL_CTL_MOD. Used so only ONE worker thread handles a given fd.
    int p[2];
    pipe(p);
    int ep = epoll_create1(0);
    struct epoll_event e = { .events = EPOLLIN | EPOLLONESHOT, .data.fd = p[0] };
    epoll_ctl(ep, EPOLL_CTL_ADD, p[0], &e);
    write(p[1], "a", 1);
    printf("ONESHOT first wait:  ready=%d\n", count_ready(ep));
    write(p[1], "b", 1);
    printf("ONESHOT second wait: ready=%d (disarmed)\n", count_ready(ep));
    epoll_ctl(ep, EPOLL_CTL_MOD, p[0], &e);
    printf("ONESHOT after MOD:   ready=%d (re-armed, data pending)\n", count_ready(ep));
    return 0;
}
