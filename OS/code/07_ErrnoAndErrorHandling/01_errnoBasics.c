#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Attempts to open a nonexistent file and demonstrates the correct
 *         convention for checking errno: only inspect errno after a
 *         library/syscall explicitly signals failure (here, open() returning
 *         -1) - errno is only meaningful right after such a failure, since a
 *         successful call is not required to reset it.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int fd;

    errno = 0;
    fd = open("/nonexistent/path/for/demo", O_RDONLY);
    if (fd == -1)
    {
        // perror() prefixes its own message and appends strerror(errno).
        perror("open failed");
        // strerror() is the manual equivalent, useful when building a
        // custom error string instead of printing straight to stderr.
        printf("errno = %d (%s)\n", errno, strerror(errno));
    }
    else
    {
        printf("unexpectedly succeeded opening the file\n");
        close(fd);
    }

    return 0;
}
