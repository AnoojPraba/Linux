#define _DEFAULT_SOURCE
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Observe glibc malloc behavior: small requests grow the heap via brk,
// large ones (>= M_MMAP_THRESHOLD, 128KB default, dynamic) get their own mmap
// and are returned to the OS on free. Freed small chunks stay in the arena,
// so RSS can stay high after free() - "memory leak" that is really fragmentation.
static long rss_kb(void)
{
    long pages = 0, rss = 0;
    FILE *f = fopen("/proc/self/statm", "r");
    if (f)
    {
        if (fscanf(f, "%ld %ld", &pages, &rss) != 2)
            rss = 0;
        fclose(f);
    }
    return rss * (sysconf(_SC_PAGESIZE) / 1024);
}

#define N 100000
int main(void)
{
    printf("start            RSS=%ld KB  brk=%p\n", rss_kb(), sbrk(0));

    void *big = malloc(1 << 20);          // 1MB: mmap'd, brk unchanged
    printf("malloc(1MB)      RSS=%ld KB  brk=%p (unchanged => mmap)\n", rss_kb(), sbrk(0));
    free(big);
    printf("free(1MB)        RSS=%ld KB  (munmap'd, returned to OS)\n", rss_kb());

    static char *p[N];
    for (int i = 0; i < N; i++)
    {
        p[i] = malloc(200);               // small: brk heap
        p[i][0] = 1;                      // touch so pages are resident
    }
    printf("100k x 200B      RSS=%ld KB  brk=%p\n", rss_kb(), sbrk(0));

    for (int i = 0; i < N; i += 2)
        free(p[i]);                       // free every other chunk
    printf("freed every 2nd  RSS=%ld KB  (holes pinned by live neighbors)\n", rss_kb());

    struct mallinfo2 mi = mallinfo2();
    printf("mallinfo2: arena=%zu KB in-use=%zu KB free=%zu KB\n", mi.arena / 1024,
           mi.uordblks / 1024, mi.fordblks / 1024);

    for (int i = 1; i < N; i += 2)
        free(p[i]);
    printf("freed all        RSS=%ld KB  (top of heap may be trimmed)\n", rss_kb());
    malloc_trim(0);
    printf("malloc_trim(0)   RSS=%ld KB\n", rss_kb());
    return 0;
}
