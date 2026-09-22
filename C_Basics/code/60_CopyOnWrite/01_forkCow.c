#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int sharedLookingValue = 100;

int main()
{
    pid_t pid;

    printf("before fork: sharedLookingValue = %d at address %p\n",
           sharedLookingValue, (void *)&sharedLookingValue);

    pid = fork();

    if (pid == 0)
    {
        // Right after fork(), the child's entire address space is a
        // logical copy of the parent's, but the kernel doesn't actually
        // duplicate any physical memory yet - both processes' page tables
        // point at the *same* physical pages, marked read-only. Only this
        // write below triggers a copy-on-write page fault: the kernel
        // notices the write, copies just that one page to a new physical
        // frame for the child, and only *then* does the child's copy
        // actually diverge from the parent's.
        sharedLookingValue = 999;
        printf("child:  after write, sharedLookingValue = %d at address %p\n",
               sharedLookingValue, (void *)&sharedLookingValue);
    }
    else
    {
        waitpid(pid, NULL, 0);
        // Same virtual address as the child printed, but a different
        // physical page underneath by now - the parent's value was never
        // touched by the child's write.
        printf("parent: after child's write, sharedLookingValue = %d at address %p\n",
               sharedLookingValue, (void *)&sharedLookingValue);
    }

    return 0;
}
