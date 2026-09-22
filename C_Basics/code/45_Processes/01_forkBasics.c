#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("fork failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        // Child: getpid() is the new process, getppid() is the parent that
        // called fork(). Both processes continue executing from this same
        // point onward, each with its own copy of the address space.
        printf("child: pid = %d, parent pid = %d\n", getpid(), getppid());
    }
    else
    {
        int status;

        printf("parent: pid = %d, spawned child pid = %d\n", getpid(), pid);
        // wait() blocks until the child exits, and reports its exit status -
        // without this, the child could finish before or after the parent
        // prints, in either order.
        waitpid(pid, &status, 0);
        printf("parent: child exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}
