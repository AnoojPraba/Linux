#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Hands-on with the TCP knobs interviewers ask about, on loopback.
//  - SO_SNDBUF/SO_RCVBUF: kernel buffers; Linux doubles the value you set.
//  - Backpressure: a non-blocking sender hits EAGAIN when the receiver does not
//    read - the send buffer + receive window are full. TCP flow control in action.
//  - TCP_NODELAY: disable Nagle (small writes sent immediately).
//  - Partial writes: write() may accept fewer bytes than asked.
static void show_opt(int fd, int level, int opt, const char *name)
{
    int v = 0; socklen_t l = sizeof v;
    getsockopt(fd, level, opt, &v, &l);
    printf("  %-14s = %d\n", name, v);
}

int main(void)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);   // rebind during TIME_WAIT
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    bind(lfd, (struct sockaddr *)&a, sizeof a);
    listen(lfd, 8);
    socklen_t al = sizeof a;
    getsockname(lfd, (struct sockaddr *)&a, &al);

    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    int small = 4096;                        // Linux doubles this internally
    setsockopt(cfd, SOL_SOCKET, SO_SNDBUF, &small, sizeof small);
    connect(cfd, (struct sockaddr *)&a, sizeof a);
    int sfd = accept(lfd, NULL, NULL);       // server side: we never read from it
    int rsmall = 4096;
    setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &rsmall, sizeof rsmall);

    setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof one);
    printf("client socket options:\n");
    show_opt(cfd, SOL_SOCKET, SO_SNDBUF, "SO_SNDBUF");
    show_opt(cfd, IPPROTO_TCP, TCP_NODELAY, "TCP_NODELAY");
    show_opt(cfd, SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE");
    printf("server socket options:\n");
    show_opt(sfd, SOL_SOCKET, SO_RCVBUF, "SO_RCVBUF");

    fcntl(cfd, F_SETFL, O_NONBLOCK);
    char chunk[1024];
    memset(chunk, 'x', sizeof chunk);
    long total = 0; int writes = 0, partial = 0;
    for (;;)
    {
        ssize_t n = write(cfd, chunk, sizeof chunk);
        if (n < 0)
        {
            if (errno == EAGAIN)
            {
                printf("write -> EAGAIN after %ld bytes in %d writes (%d partial): "
                       "receiver not reading => buffers + window full => backpressure\n",
                       total, writes, partial);
                break;
            }
            perror("write");
            break;
        }
        if (n < (ssize_t)sizeof chunk)
            partial++;
        total += n; writes++;
    }

    // Draining the receiver reopens the window and the sender can continue.
    char buf[65536]; long drained = 0; ssize_t r;
    fcntl(sfd, F_SETFL, O_NONBLOCK);
    while ((r = read(sfd, buf, sizeof buf)) > 0)
        drained += r;
    printf("receiver drained %ld bytes; sender writable again: %s\n", drained,
           write(cfd, chunk, 1) == 1 ? "yes" : "no");

    struct tcp_info ti; socklen_t tl = sizeof ti;
    if (getsockopt(cfd, IPPROTO_TCP, TCP_INFO, &ti, &tl) == 0)
        printf("TCP_INFO: state=%u rtt=%uus cwnd=%u segs mss=%u retrans=%u\n",
               ti.tcpi_state, ti.tcpi_rtt, ti.tcpi_snd_cwnd, ti.tcpi_snd_mss,
               ti.tcpi_total_retrans);
    close(cfd); close(sfd); close(lfd);
    return 0;
}
