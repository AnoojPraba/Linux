#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

// UDP gives you: datagram boundaries preserved, no connection, no reliability,
// no ordering, no flow/congestion control. Two things shown here:
//  1. Message boundaries: each recvfrom() returns exactly ONE datagram (a short
//     buffer truncates the rest - unlike TCP, which would return a stream).
//  2. Reliability built by the application: request id + timeout + retry with
//     exponential backoff. The server DROPS the first two requests on purpose
//     (simulated packet loss) so you can watch retries work. Idempotent
//     requests (or server-side dedup by id) make retries safe.
static int server_fd, port;

static void *server(void *arg)
{
    (void)arg;
    int seen = 0;
    for (;;)
    {
        char buf[128];
        struct sockaddr_in from;
        socklen_t fl = sizeof from;
        ssize_t n = recvfrom(server_fd, buf, sizeof buf - 1, 0, (struct sockaddr *)&from, &fl);
        if (n < 0)
            break;
        buf[n] = '\0';
        if (strcmp(buf, "quit") == 0)
            break;
        if (seen++ < 2)
        {
            printf("  server: DROPPING \"%s\" (simulated loss)\n", buf);
            continue;
        }
        char reply[160];
        int len = snprintf(reply, sizeof reply, "ok:%s", buf);
        sendto(server_fd, reply, len, 0, (struct sockaddr *)&from, fl);
    }
    return NULL;
}

int main(void)
{
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = { .sin_family = AF_INET,
                                .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    bind(server_fd, (struct sockaddr *)&addr, sizeof addr);
    socklen_t al = sizeof addr;
    getsockname(server_fd, (struct sockaddr *)&addr, &al);
    port = ntohs(addr.sin_port);
    pthread_t t;
    pthread_create(&t, NULL, server, NULL);

    int c = socket(AF_INET, SOCK_DGRAM, 0);
    connect(c, (struct sockaddr *)&addr, sizeof addr);   // "connected" UDP: send/recv,
                                                          // only this peer's datagrams delivered,
                                                          // and ICMP errors surface as ECONNREFUSED
    // ---- 1. retry with exponential backoff ----
    long timeout_ms = 50;
    char reply[160] = {0};
    for (int attempt = 1; attempt <= 5; attempt++)
    {
        struct timeval tv = { .tv_sec = 0, .tv_usec = timeout_ms * 1000 };
        setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
        send(c, "req-42", 6, 0);
        ssize_t n = recv(c, reply, sizeof reply - 1, 0);
        if (n >= 0)
        {
            reply[n] = '\0';
            printf("attempt %d (timeout %ldms): got \"%s\"\n", attempt, timeout_ms, reply);
            break;
        }
        printf("attempt %d (timeout %ldms): no reply (%s) -> retry\n", attempt, timeout_ms,
               strerror(errno));
        timeout_ms *= 2;
    }

    // ---- 2. datagram boundaries ----
    struct sockaddr_in me;
    int rx = socket(AF_INET, SOCK_DGRAM, 0);
    me.sin_family = AF_INET; me.sin_addr.s_addr = htonl(INADDR_LOOPBACK); me.sin_port = 0;
    bind(rx, (struct sockaddr *)&me, sizeof me);
    socklen_t ml = sizeof me;
    getsockname(rx, (struct sockaddr *)&me, &ml);
    int tx = socket(AF_INET, SOCK_DGRAM, 0);
    sendto(tx, "AAAA", 4, 0, (struct sockaddr *)&me, sizeof me);
    sendto(tx, "BBBBBBBBBB", 10, 0, (struct sockaddr *)&me, sizeof me);
    sendto(tx, "CC", 2, 0, (struct sockaddr *)&me, sizeof me);
    char small[5];
    for (int i = 0; i < 3; i++)
    {
        ssize_t n = recvfrom(rx, small, sizeof small, MSG_TRUNC, NULL, NULL);
        printf("datagram %d: real size %zd bytes (buffer holds %zu -> %s)\n", i + 1, n,
               sizeof small, n > (ssize_t)sizeof small ? "TRUNCATED" : "complete");
    }

    sendto(c, "quit", 4, 0, NULL, 0);
    pthread_join(t, NULL);
    return 0;
}
