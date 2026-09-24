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
        // execvp replaces the child's entire address space with a new
        // program - if it succeeds, none of this process's own code after
        // the call ever runs (there is no "return" from a successful exec).
        char *args[] = {"echo", "hello", "from", "exec", NULL};

        execvp("echo", args);

        // Only reached if execvp itself failed to start the new program.
        printf("execvp failed\n");
        return 1;
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("parent: child finished running echo\n");
    }

    return 0;
}
