// First fit with split and coalescing of neighbouring holes.

#include <stdio.h>

#define MAXB 16

typedef struct
{
    int start, size, used;          /* one block of the arena, in address order */
} Block;

static Block blk[MAXB];
static int nb;

static int alloc(int size)
{
    for (int i = 0; i < nb; i++)
        if (!blk[i].used && blk[i].size >= size)         /* first fit */
        {
            if (blk[i].size > size)                      /* split: the rest stays free */
            {
                for (int j = nb; j > i + 1; j--)
                    blk[j] = blk[j - 1];
                blk[i + 1] = (Block){blk[i].start + size, blk[i].size - size, 0};
                nb++;
                blk[i].size = size;
            }
            blk[i].used = 1;
            return blk[i].start;
        }
    return -1;                                            /* no hole is big enough */
}

static void release(int start)
{
    for (int i = 0; i < nb; i++)
        if (blk[i].start == start)
        {
            blk[i].used = 0;
            if (i + 1 < nb && !blk[i + 1].used)           /* merge with the next block */
            {
                blk[i].size += blk[i + 1].size;
                for (int j = i + 1; j + 1 < nb; j++) blk[j] = blk[j + 1];
                nb--;
            }
            if (i > 0 && !blk[i - 1].used)                /* merge with the previous block */
            {
                blk[i - 1].size += blk[i].size;
                for (int j = i; j + 1 < nb; j++) blk[j] = blk[j + 1];
                nb--;
            }
            return;
        }
}

static void show(const char *what)
{
    printf("%-14s", what);
    for (int i = 0; i < nb; i++)
        printf("[%s %d@%d] ", blk[i].used ? "used" : "free", blk[i].size, blk[i].start);
    printf("\n");
}

int main(void)
{
    blk[0] = (Block){0, 100, 0};
    nb = 1;
    show("start:");
    int a = alloc(30), b = alloc(20), c = alloc(10);
    show("after 3 allocs:");
    release(b);
    show("free b:");
    release(a);
    show("free a:");
    printf("alloc 45 -> %d (needs a bigger hole)\n", alloc(45));
    release(c);
    show("free c:");
    printf("alloc 45 -> %d\n", alloc(45));
    return 0;
}
