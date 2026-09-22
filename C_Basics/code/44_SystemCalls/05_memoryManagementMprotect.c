#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

void handleSegv(int signum)
{
    (void)signum;
    // Only safe operations in a signal handler (see 47_SignalHandling) -
    // write() is reentrant-safe, unlike printf.
    const char *message = "caught SIGSEGV: write to read-only page blocked\n";

    write(STDOUT_FILENO, message, strlen(message));
    // A page-protection violation is unrecoverable at the faulting
    // instruction, so the handler must end the program rather than return.
    _exit(0);
}

int main()
{
    long pageSize = sysconf(_SC_PAGESIZE);
    char *region = mmap(NULL, pageSize, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    signal(SIGSEGV, handleSegv);

    region[0] = 'A';
    printf("wrote to region while writable: %c\n", region[0]);

    // mprotect changes the access permissions of already-mapped pages -
    // the memory management unit (MMU) will now fault on any write,
    // regardless of what the process's code tries to do.
    mprotect(region, pageSize, PROT_READ);
    printf("mprotect'd region to read-only, reading still works: %c\n", region[0]);

    printf("attempting a write to the now read-only region...\n");
    region[0] = 'B';

    printf("this line should never print\n");
    return 0;
}
