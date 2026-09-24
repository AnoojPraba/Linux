#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_KEY 0x1234

int main()
{
    // System V shared memory: shmget() creates (or attaches to) a shared
    // segment identified by a key, separate from the POSIX
    // mmap(MAP_SHARED|MAP_ANONYMOUS) approach in 45_IPC/02_sharedMemory.c -
    // this segment is a kernel-managed object with its own lifetime,
    // outliving any single process, until explicitly removed with
    // shmctl(IPC_RMID).
    int shmId = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    int *sharedValue;
    pid_t pid;

    // shmat() attaches the segment into this process's address space,
    // returning a regular pointer usable like any other.
    sharedValue = (int *)shmat(shmId, NULL, 0);
    *sharedValue = 0;

    pid = fork();
    if (pid == 0)
    {
        *sharedValue = 99;
        shmdt(sharedValue);
        return 0;
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("parent sees sharedValue = %d (written by child)\n", *sharedValue);
        shmdt(sharedValue);
        // IPC_RMID marks the segment for removal once every process has
        // detached from it - without this, the segment would linger in the
        // kernel after this program exits.
        shmctl(shmId, IPC_RMID, NULL);
    }

    return 0;
}
