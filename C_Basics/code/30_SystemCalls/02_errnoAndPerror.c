#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    // A syscall/library function signals failure through its return value
    // (here, -1), then leaves the specific reason in the global errno -
    // the return value alone can't distinguish "file missing" from
    // "permission denied" from any other failure.
    int fd = open("/nonexistent/path/for/this/demo", O_RDONLY);

    if (fd < 0)
    {
        // errno must be saved immediately - any later library call
        // (even a successful one, like printf) can overwrite it as a side
        // effect, so reading errno again afterward is not reliable.
        int savedErrno = errno;

        // perror prints a caller-supplied prefix plus the human-readable
        // message for the current errno value.
        perror("open failed");
        printf("errno = %d\n", savedErrno);

        if (savedErrno == ENOENT)
        {
            printf("that specific error means: No such file or directory\n");
        }
    }
    else
    {
        close(fd);
    }

    return 0;
}
