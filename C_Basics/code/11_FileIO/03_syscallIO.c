#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_PATH "/tmp/c_basics_syscall_io_demo.txt"
#define READ_BUF_SIZE 64

int main()
{
    // These are raw kernel system calls (unistd.h/fcntl.h), unlike fopen()
    // etc. in 01_writeRead.c, which are buffered library functions layered
    // on top of these same syscalls. open() returns a plain int file
    // descriptor, not a FILE *.
    int fd = open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    const char *message = "raw syscall write\n";
    char buffer[READ_BUF_SIZE];
    ssize_t bytesWritten;
    ssize_t bytesRead;
    off_t offset;

    if (fd < 0)
    {
        printf("open for write failed\n");
        return 1;
    }
    bytesWritten = write(fd, message, 18);
    printf("wrote %zd bytes\n", bytesWritten);
    close(fd);

    fd = open(FILE_PATH, O_RDONLY);
    if (fd < 0)
    {
        printf("open for read failed\n");
        return 1;
    }

    bytesRead = read(fd, buffer, READ_BUF_SIZE - 1);
    buffer[bytesRead] = '\0';
    printf("read %zd bytes: %s", bytesRead, buffer);

    // lseek repositions the file offset without reading/writing - here,
    // back to the start, so the next read() starts over from byte 0.
    offset = lseek(fd, 0, SEEK_SET);
    printf("lseek returned offset %ld\n", (long)offset);

    bytesRead = read(fd, buffer, 3);
    buffer[bytesRead] = '\0';
    printf("first 3 bytes after seek: %s\n", buffer);

    close(fd);
    return 0;
}
