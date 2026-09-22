#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("child (pid %d): exiting immediately\n", getpid());
        return 0;
    }
    else
    {
        // A child that has exited but whose parent hasn't called wait()
        // yet becomes a zombie - the kernel keeps its exit status around
        // (visible in `ps` as state Z) until the parent collects it.
        // Sleeping here (instead of calling waitpid immediately) is what
        // gives a window to observe that state.
        printf("parent: sleeping for 2 seconds - check `ps -o pid,ppid,state,cmd -p %d`"
               " in another terminal to see the zombie\n", pid);
        sleep(2);

        // waitpid() here reaps the zombie, letting the kernel finally
        // discard its exit status.
        waitpid(pid, NULL, 0);
        printf("parent: reaped the zombie child\n");
    }

    return 0;
}
