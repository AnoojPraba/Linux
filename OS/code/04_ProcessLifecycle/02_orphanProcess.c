#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // The parent below exits almost immediately, well before this
        // sleep finishes - once that happens, this child becomes an
        // orphan and is re-parented to init/PID 1 (or the nearest
        // subreaper), rather than being left parentless.
        printf("child (pid %d): parent pid right after fork = %d\n", getpid(), getppid());
        sleep(1);
        printf("child (pid %d): parent pid after parent exited = %d"
               " (no longer the original parent)\n", getpid(), getppid());
    }
    else
    {
        printf("parent (pid %d): exiting immediately, orphaning the child\n", getpid());
    }

    return 0;
}
