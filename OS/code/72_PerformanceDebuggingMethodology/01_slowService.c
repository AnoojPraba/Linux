#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

// Two classic accidental slowdowns, each with its fix, timed side by side.
// Run `strace -c ./01_slowService slow` to see the syscall-count signature of
// bug #2, and `perf record -g` (where available) to see bug #1 as a hot
// strlen/strcpy. Usage: 01_slowService [slow|fast]
static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Bug 1: strlen() in the loop condition is O(n) each time -> O(n^2).
static long count_commas_slow(const char *s)
{
    long c = 0;
    for (size_t i = 0; i < strlen(s); i++)
        if (s[i] == ',')
            c++;
    return c;
}
static long count_commas_fast(const char *s)
{
    long c = 0;
    for (; *s; s++)
        if (*s == ',')
            c++;
    return c;
}

// Bug 2: one write() syscall per tiny record -> syscall-bound (kernel time).
static void log_slow(int fd, int n)
{
    char line[32];
    for (int i = 0; i < n; i++)
    {
        int len = snprintf(line, sizeof line, "event %d\n", i);
        if (write(fd, line, len) < 0)
            return;
    }
}
static void log_fast(int fd, int n)          // batch into one buffer
{
    static char buf[1 << 16];
    size_t used = 0;
    for (int i = 0; i < n; i++)
    {
        int len = snprintf(buf + used, sizeof buf - used, "event %d\n", i);
        used += len;
        if (used > sizeof buf - 32)
        {
            if (write(fd, buf, used) < 0)
                return;
            used = 0;
        }
    }
    if (used && write(fd, buf, used) < 0)
        return;
}

int main(int argc, char **argv)
{
    int slow = !(argc > 1 && !strcmp(argv[1], "fast"));
    size_t n = 50000;
    char *s = malloc(n + 1);
    for (size_t i = 0; i < n; i++)
        s[i] = (i % 7 == 0) ? ',' : 'a';
    s[n] = '\0';

    double t = now();
    long c = slow ? count_commas_slow(s) : count_commas_fast(s);
    printf("[%s] 1. strlen in loop     : %.4fs (commas=%ld)\n", slow ? "slow" : "fast", now() - t, c);

    int fd = open("/dev/null", O_WRONLY);
    t = now();
    if (slow) log_slow(fd, 200000); else log_fast(fd, 200000);
    printf("[%s] 2. write per record   : %.4fs\n", slow ? "slow" : "fast", now() - t);

    return 0;
}
