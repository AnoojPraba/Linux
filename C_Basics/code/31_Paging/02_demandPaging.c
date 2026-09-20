#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define NUM_PAGES 512

// Reads this process's own resident set size (RSS: physical memory actually
// backing this process right now) from /proc/self/status, in kilobytes.
long readRssKb(void)
{
    FILE *fp = fopen("/proc/self/status", "r");
    char line[256];
    long rssKb = -1;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "VmRSS: %ld kB", &rssKb) == 1)
        {
            break;
        }
    }
    fclose(fp);
    return rssKb;
}

int main()
{
    long pageSize = sysconf(_SC_PAGESIZE);
    size_t mapLength = NUM_PAGES * pageSize;
    char *region;
    int i;

    printf("RSS before mmap: %ld kB\n", readRssKb());

    // MAP_ANONYMOUS memory is demand-paged: mmap only reserves the virtual
    // address range here - no physical memory is actually assigned to any
    // page yet, which is why RSS barely moves even though a large region
    // was just "allocated".
    region = mmap(NULL, mapLength, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    printf("RSS right after mmap (%zu bytes reserved): %ld kB\n",
           mapLength, readRssKb());

    // Touching a page for the first time triggers a page fault: the kernel
    // notices there's no physical frame behind this virtual page yet,
    // assigns one, and only then does RSS grow - one page (pageSize bytes)
    // at a time, not the whole mapping at once.
    for (i = 0; i < NUM_PAGES; i++)
    {
        region[i * pageSize] = 1;
    }

    printf("RSS after touching all %d pages: %ld kB\n", NUM_PAGES, readRssKb());

    munmap(region, mapLength);
    return 0;
}
