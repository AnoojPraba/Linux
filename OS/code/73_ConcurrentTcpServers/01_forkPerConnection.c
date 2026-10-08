#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

// Classic Unix concurrency model: accept() then fork() a child per connection.
// Strong isolation (a crash/leak in one handler cannot corrupt others), simple
// code, but costs a process per client and fork latency. Used by early Apache
// (prefork), PostgreSQL (process-per-connection), sshd.
//
// Zombies: a child that exits stays a zombie until the parent wait()s. Reap in
// a SIGCHLD handler with a waitpid(WNOHANG) LOOP (signals coalesce, so one
// signal can mean several dead children).
#define NCLIENTS 3

static void on_sigchld(int sig)
{
    (void)sig;
    int saved = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved;
}

static void handle_client(int fd)
{
    char buf[128];
    ssize_t n = read(fd, buf, sizeof buf - 1);
    if (n > 0)
    {
        buf[n] = '\0';
        char reply[192];
        int len = snprintf(reply, sizeof reply, "pid %d handled: %s", getpid(), buf);
        write(fd, reply, len);
    }
    close(fd);
}

static void run_client(int port, int id)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_port = htons(port),
                             .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    if (connect(fd, (struct sockaddr *)&a, sizeof a) < 0)
        _exit(1);
    char msg[32], reply[256] = {0};
    int n = snprintf(msg, sizeof msg, "client %d", id);
    write(fd, msg, n);
    read(fd, reply, sizeof reply - 1);
    printf("client %d <- \"%s\"\n", id, reply);
    fflush(stdout);                 // _exit() skips the stdio flush; without this the
    close(fd);                      // line is lost when stdout is a pipe/file
    _exit(0);
}

int main(void)
{
    struct sigaction sa = { .sa_handler = on_sigchld, .sa_flags = SA_RESTART };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int one = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
    struct sockaddr_in addr = { .sin_family = AF_INET,
                                .sin_addr.s_addr = htonl(INADDR_LOOPBACK) };
    bind(lfd, (struct sockaddr *)&addr, sizeof addr);
    listen(lfd, 16);
    socklen_t len = sizeof addr;
    getsockname(lfd, (struct sockaddr *)&addr, &len);
    int port = ntohs(addr.sin_port);
    printf("parent pid %d listening on port %d\n", getpid(), port);
    fflush(stdout);                 // flush BEFORE fork or children inherit a copy of the buffer

    for (int i = 0; i < NCLIENTS; i++)
        if (fork() == 0)
        {
            close(lfd);
            run_client(port, i);
        }

    for (int i = 0; i < NCLIENTS; i++)
    {
        int cfd = accept(lfd, NULL, NULL);
        if (cfd < 0)
        {
            if (errno == EINTR) { i--; continue; }   // SIGCHLD interrupted accept
            break;
        }
        if (fork() == 0)
        {
            close(lfd);                 // child doesn't need the listener
            handle_client(cfd);
            _exit(0);
        }
        close(cfd);                     // parent must close its copy or the fd leaks
    }
    while (wait(NULL) > 0 || errno == EINTR)    // wait for all clients and handlers
        ;
    close(lfd);
    return 0;
}
