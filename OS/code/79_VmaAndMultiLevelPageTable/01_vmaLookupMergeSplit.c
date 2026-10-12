// Sorted VMA array with binary search; adjacent mappings with equal
// permissions merge; munmap/mprotect in the middle splits.

#include <stdio.h>
#include <string.h>

typedef struct
{
    unsigned long start, end;                     /* [start, end) */
    char perm[4];
} Vma;

static Vma vma[16];
static int n;

static int find(unsigned long addr)               /* binary search: index of the VMA containing addr, or -1 */
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (addr < vma[mid].start) hi = mid - 1;
        else if (addr >= vma[mid].end) lo = mid + 1;
        else return mid;
    }
    return -1;
}

static void insert(unsigned long start, unsigned long end, const char *perm)
{
    int i = 0;
    while (i < n && vma[i].start < start)
        i++;
    memmove(&vma[i + 1], &vma[i], (size_t)(n - i) * sizeof vma[0]);
    vma[i] = (Vma){start, end, {0}};
    strcpy(vma[i].perm, perm);
    n++;
    if (i + 1 < n && vma[i].end == vma[i + 1].start && !strcmp(vma[i].perm, vma[i + 1].perm))
    {
        vma[i].end = vma[i + 1].end;              /* merge with the next neighbour */
        memmove(&vma[i + 1], &vma[i + 2], (size_t)(n - i - 2) * sizeof vma[0]);
        n--;
    }
    if (i > 0 && vma[i - 1].end == vma[i].start && !strcmp(vma[i - 1].perm, vma[i].perm))
    {
        vma[i - 1].end = vma[i].end;              /* merge with the previous neighbour */
        memmove(&vma[i], &vma[i + 1], (size_t)(n - i - 1) * sizeof vma[0]);
        n--;
    }
}

static void unmap(unsigned long start, unsigned long end)    /* the range must lie inside one VMA */
{
    int i = find(start);
    if (i < 0) return;
    Vma v = vma[i];
    memmove(&vma[i], &vma[i + 1], (size_t)(n - i - 1) * sizeof vma[0]);
    n--;
    if (v.start < start) insert(v.start, start, v.perm);     /* keep the part before the hole */
    if (end < v.end) insert(end, v.end, v.perm);             /* keep the part after the hole  */
}

static void show(const char *what)
{
    printf("%s:", what);
    for (int i = 0; i < n; i++)
        printf(" [%lx-%lx %s]", vma[i].start, vma[i].end, vma[i].perm);
    printf("  (%d VMAs)\n", n);
}

int main(void)
{
    insert(0x1000, 0x3000, "rw-");
    insert(0x8000, 0x9000, "r-x");
    show("two mappings   ");
    insert(0x3000, 0x5000, "rw-");
    show("adjacent rw    ");
    unmap(0x2000, 0x3000);
    show("munmap middle  ");
    printf("find 0x4000 -> VMA %d, find 0x2800 -> VMA %d, find 0x8800 -> VMA %d\n", find(0x4000), find(0x2800), find(0x8800));
    return 0;
}
