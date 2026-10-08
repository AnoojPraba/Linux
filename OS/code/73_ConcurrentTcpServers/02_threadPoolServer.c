#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Thread pool + bounded work queue: accept() on the main thread pushes client
// fds into a queue; a fixed set of workers pops and serves them. Compared with
// thread-per-connection it caps resource use (threads, stacks) and gives
// natural backpressure: when the queue is full the acceptor blocks (or sheds
// load), instead of spawning unbounded threads.
#define NWORKERS 3
#define QCAP 8
#define NCLIENTS 6

static int queue[QCAP];
static int qhead, qtail, qcount;
static int shutting_down;
static pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

static void queue_push(int fd)
{
    pthread_mutex_lock(&qlock);
    while (qcount == QCAP)
        pthread_cond_wait(&not_full, &qlock);       // backpressure on the acceptor
    queue[qtail] = fd;
    qtail = (qtail + 1) % QCAP;
    qcount++;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&qlock);
}

static int queue_pop(void)                          // -1 when shutting down and empty
{
    pthread_mutex_lock(&qlock);
    while (qcount == 0 && !shutting_down)
        pthread_cond_wait(&not_empty, &qlock);
    int fd = -1;
    if (qcount > 0)
    {
        fd = queue[qhead];
        qhead = (qhead + 1) % QCAP;
        qcount--;
        pthread_cond_signal(&not_full);
    }
    pthread_mutex_unlock(&qlock);
    return fd;
}

static void *worker(void *arg)
{
    long id = (long)arg;
    int fd;
    while ((fd = queue_pop()) >= 0)
    {
        char buf[64];
        ssize_t n = read(fd, buf, sizeof buf - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            char reply[128];
            int len = snprintf(reply, sizeof reply, "worker %ld served \"%s\"", id, buf);
            write(fd, reply, len);
        }
        close(fd);
    }
    return NULL;
}

static int port;

static void *client(void *arg)
{
    long id = (long)arg;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_port = htons(port),
                             .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    if (connect(fd, (struct sockaddr *)&a, sizeof a) == 0)
    {
        char msg[32], reply[128] = {0};
        int n = snprintf(msg, sizeof msg, "req %ld", id);
        write(fd, msg, n);
        read(fd, reply, sizeof reply - 1);
        pthread_mutex_lock(&print_lock);
        printf("client %ld <- %s\n", id, reply);
        pthread_mutex_unlock(&print_lock);
    }
    close(fd);
    return NULL;
}

int main(void)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
    struct sockaddr_in addr = { .sin_family = AF_INET,
                                .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    bind(lfd, (struct sockaddr *)&addr, sizeof addr);
    listen(lfd, 16);
    socklen_t len = sizeof addr;
    getsockname(lfd, (struct sockaddr *)&addr, &len);
    port = ntohs(addr.sin_port);

    pthread_t w[NWORKERS], c[NCLIENTS];
    for (long i = 0; i < NWORKERS; i++)
        pthread_create(&w[i], NULL, worker, (void *)i);
    for (long i = 0; i < NCLIENTS; i++)
        pthread_create(&c[i], NULL, client, (void *)i);

    for (int i = 0; i < NCLIENTS; i++)
        queue_push(accept(lfd, NULL, NULL));

    for (int i = 0; i < NCLIENTS; i++)
        pthread_join(c[i], NULL);

    pthread_mutex_lock(&qlock);                 // graceful shutdown: drain, then stop
    shutting_down = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&qlock);
    for (int i = 0; i < NWORKERS; i++)
        pthread_join(w[i], NULL);
    close(lfd);
    puts("server shut down cleanly");
    return 0;
}
