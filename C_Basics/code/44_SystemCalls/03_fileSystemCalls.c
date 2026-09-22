#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIR_PATH "/tmp/c_basics_syscall_dir_demo"
#define FILE_PATH "/tmp/c_basics_syscall_dir_demo/file.txt"

int main()
{
    struct stat statInfo;
    int fd;

    // mkdir/creat/rename/unlink are the file-system-metadata syscalls,
    // distinct from the open/read/write/close/lseek data syscalls covered
    // in 26_FileIO/03_syscallIO.c - these operate on directory entries and
    // file attributes, not on a file's byte contents.
    mkdir(DIR_PATH, 0755);

    fd = creat(FILE_PATH, 0644);
    close(fd);

    // stat() fills in a file's metadata - size, permissions, timestamps -
    // without opening it at all.
    if (stat(FILE_PATH, &statInfo) == 0)
    {
        printf("file size = %ld bytes\n", (long)statInfo.st_size);
        printf("is regular file = %d\n", S_ISREG(statInfo.st_mode));
        printf("is directory = %d\n", S_ISDIR(statInfo.st_mode));
    }

    if (stat(DIR_PATH, &statInfo) == 0)
    {
        printf("DIR_PATH is a directory = %d\n", S_ISDIR(statInfo.st_mode));
    }

    // Removing a directory entry (unlink) is a separate syscall from
    // removing directories (rmdir) - a directory can't be unlink()'d
    // directly on Linux.
    unlink(FILE_PATH);
    rmdir(DIR_PATH);

    printf("cleaned up demo file and directory\n");
    return 0;
}
