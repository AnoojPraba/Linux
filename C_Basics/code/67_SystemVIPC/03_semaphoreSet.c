#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEM_KEY 0x9abc

// System V semaphores come in sets (even a "single" semaphore is a
// 1-element set) - operations act on one member of the set by index,
// unlike the single POSIX sem_t used in 66_IPC/03_processSemaphore.c.
void semOp(int semId, int semNum, int op)
{
    struct sembuf operation;

    operation.sem_num = semNum;
    operation.sem_op = op;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
}

int main()
{
    int semId = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    pid_t pid;

    // Initialize the semaphore's value directly via semctl - System V
    // semaphores don't start at a known value the way sem_init()'s third
    // argument sets one explicitly.
    semctl(semId, 0, SETVAL, 0);

    pid = fork();
    if (pid == 0)
    {
        printf("child: doing setup work\n");
        sleep(1);
        printf("child: signaling parent\n");
        // A positive op increments the semaphore (signal/post).
        semOp(semId, 0, 1);
        return 0;
    }
    else
    {
        printf("parent: waiting for child to finish setup\n");
        // A negative op decrements the semaphore, blocking while it would
        // go below zero (wait) - the process-synchronization equivalent of
        // the POSIX sem_wait() in 66_IPC/03_processSemaphore.c.
        semOp(semId, 0, -1);
        printf("parent: child signaled, continuing\n");

        waitpid(pid, NULL, 0);
        semctl(semId, 0, IPC_RMID);
    }

    return 0;
}
