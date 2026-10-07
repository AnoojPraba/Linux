#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// A handler may interrupt the program at ANY instruction - including inside
// malloc or printf holding an internal lock. So a handler may only call
// async-signal-safe functions (write, _exit, kill, sigaction, ...) and touch
// only `volatile sig_atomic_t` / lock-free atomics.
static volatile sig_atomic_t got_signal = 0;
static int self_pipe[2];

static void handler(int sig)
{
    int saved_errno = errno;       // handlers must preserve errno
    got_signal = sig;
    static const char msg[] = "handler: caught signal (write is safe)\n";
    (void)!write(STDOUT_FILENO, msg, sizeof msg - 1);   // NOT printf
    unsigned char b = (unsigned char)sig;
    (void)!write(self_pipe[1], &b, 1);   // self-pipe trick: wake the main loop
    errno = saved_errno;
}

int main(void)
{
    if (pipe(self_pipe) < 0)
        return 1;

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;        // restart interrupted slow syscalls
    sigaction(SIGUSR1, &sa, NULL);   // sigaction, not signal(): portable semantics

    printf("pid=%d, raising SIGUSR1\n", getpid());
    fflush(stdout);
    raise(SIGUSR1);

    // Real work happens here in normal context, where printf/malloc are fine.
    unsigned char b;
    if (read(self_pipe[0], &b, 1) == 1)
        printf("main: processed deferred signal %d (flag=%d)\n", b, got_signal);

    // Blocking signals around a critical section: pending signals are
    // delivered when unblocked.
    sigset_t block, old;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block, &old);
    raise(SIGUSR1);
    printf("main: signal blocked, pending until unmask\n");
    sigprocmask(SIG_SETMASK, &old, NULL);   // handler runs here

    read(self_pipe[0], &b, 1);
    printf("main: done\n");
    return 0;
}
