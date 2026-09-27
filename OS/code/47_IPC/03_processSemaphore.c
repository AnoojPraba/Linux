#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    // A sem_t placed in mmap'd MAP_SHARED memory (not a stack/heap variable)
    // is what makes it usable across a fork() - sem_init's second argument
    // (pshared) must be nonzero for that to be valid.
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid_t pid;

    sem_init(sem, 1, 0);

    pid = fork();

    if (pid == 0)
    {
        printf("child: doing setup work\n");
        sleep(1);
        printf("child: signaling parent\n");
        sem_post(sem);
    }
    else
    {
        printf("parent: waiting for child to finish setup\n");
        sem_wait(sem);
        printf("parent: child signaled, continuing\n");
        waitpid(pid, NULL, 0);
        sem_destroy(sem);
    }

    munmap(sem, sizeof(sem_t));
    return 0;
}
