#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_PATH "/tmp/c_basics_mmap_file_demo.txt"

int main()
{
    const char *initialContent = "0123456789";
    int fd;
    struct stat statInfo;
    char *mapped;

    fd = open(FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, initialContent, 10);

    fstat(fd, &statInfo);

    // Unlike the anonymous mappings in 29_Paging/ (backed by nothing, just
    // zero-filled pages), this mapping is backed by an actual file - reads
    // through "mapped" pull from the file's contents, and writes through
    // it modify the file directly, without any explicit read()/write()
    // calls at all.
    mapped = mmap(NULL, statInfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    printf("file contents via mmap: %.*s\n", (int)statInfo.st_size, mapped);

    // Modify byte 0 directly through the mapping.
    mapped[0] = 'X';

    // msync flushes this change back to the file's on-disk contents -
    // MAP_SHARED writes are eventually written back by the kernel on their
    // own, but msync forces it synchronously right now.
    msync(mapped, statInfo.st_size, MS_SYNC);
    munmap(mapped, statInfo.st_size);

    // Reopen and read normally to prove the write landed in the actual file,
    // not just in this process's mapped memory.
    fd = open(FILE_PATH, O_RDONLY);
    {
        char buffer[16];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

        buffer[bytesRead] = '\0';
        printf("file contents after re-reading from disk: %s\n", buffer);
    }
    close(fd);

    return 0;
}
