#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define NUM_PAGES 3

int main()
{
    // The kernel manages memory in fixed-size chunks called pages (4 KB on
    // most x86_64 Linux systems) - this is queried at runtime, not a
    // compile-time constant, because it can differ across architectures.
    long pageSize = sysconf(_SC_PAGESIZE);
    size_t mapLength = NUM_PAGES * pageSize;
    char *region;
    int i;

    printf("page size = %ld bytes\n", pageSize);
    printf("mapping %d pages = %zu bytes\n", NUM_PAGES, mapLength);

    // mmap always allocates in whole pages - even mmap(1) would actually
    // reserve one full page, since a page is the smallest unit the virtual
    // memory system can hand out or protect.
    region = mmap(NULL, mapLength, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Write one byte at the start of each page - each page is a separate
    // unit of memory the OS can back with a physical frame independently,
    // demonstrated further in 02_demandPaging.c.
    for (i = 0; i < NUM_PAGES; i++)
    {
        region[i * pageSize] = (char)('A' + i);
    }

    printf("first byte of each page: ");
    for (i = 0; i < NUM_PAGES; i++)
    {
        printf("%c ", region[i * pageSize]);
    }
    printf("\n");

    munmap(region, mapLength);
    return 0;
}
