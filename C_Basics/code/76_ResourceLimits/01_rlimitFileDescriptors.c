#include <stdio.h>
#include <fcntl.h>
#include <sys/resource.h>

#define LOWERED_FD_LIMIT 10

int main()
{
    struct rlimit limit;
    int openedCount = 0;
    int fd;

    // Every process has a "soft" limit (the current enforced cap, which
    // this process can lower or raise up to the hard limit) and a "hard"
    // limit (the ceiling the soft limit can be raised to, only raisable
    // further by root) for each resource - RLIMIT_NOFILE is the count of
    // simultaneously open file descriptors.
    getrlimit(RLIMIT_NOFILE, &limit);
    printf("current RLIMIT_NOFILE: soft = %ld, hard = %ld\n",
           (long)limit.rlim_cur, (long)limit.rlim_max);

    // Lower the soft limit dramatically for this demo, so hitting it
    // doesn't require actually opening thousands of real files.
    limit.rlim_cur = LOWERED_FD_LIMIT;
    setrlimit(RLIMIT_NOFILE, &limit);
    printf("lowered soft limit to %d\n", LOWERED_FD_LIMIT);

    // Keep opening /dev/null (never fails for its own reasons) until the
    // lowered limit itself is what causes open() to fail - the exact
    // mechanism that makes "too many open files" (EMFILE) a real failure
    // mode in long-running services that leak file descriptors over time.
    while ((fd = open("/dev/null", O_RDONLY)) != -1)
    {
        openedCount++;
    }

    printf("opened %d file descriptors before hitting the limit (open() failed"
           " with errno indicating EMFILE)\n", openedCount);

    // A fork bomb (a process that repeatedly fork()s itself with no exit
    // condition) is the same category of resource-exhaustion problem, but
    // for RLIMIT_NPROC (process count) instead of file descriptors -
    // deliberately not demonstrated here, since actually running one
    // would exhaust this machine's process table for every user on it,
    // not just this program. setrlimit(RLIMIT_NPROC, ...) - typically
    // configured system-wide via /etc/security/limits.conf - is the
    // standard mitigation, capping how many processes a single user can
    // ever have running at once.
    return 0;
}
