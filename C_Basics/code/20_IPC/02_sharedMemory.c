#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    // MAP_ANONYMOUS | MAP_SHARED gives a memory region backed by no file but
    // still shared (not copy-on-write) across a fork - unlike ordinary heap
    // memory, both parent and child see writes the other makes to it.
    int *sharedValue = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid_t pid;

    *sharedValue = 0;
    pid = fork();

    if (pid == 0)
    {
        *sharedValue = 42;
        munmap(sharedValue, sizeof(int));
        return 0;
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("parent sees sharedValue = %d (written by child)\n", *sharedValue);
        munmap(sharedValue, sizeof(int));
    }

    return 0;
}
