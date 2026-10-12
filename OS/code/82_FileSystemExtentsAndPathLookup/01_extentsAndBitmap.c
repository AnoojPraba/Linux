// ctz first-zero allocation from a bitmap, extent list growth and merge, file
// offset to disk block lookup, interleaving fragmentation.

#include <stdio.h>

#define BLOCKS 64

static unsigned long bitmap;                      /* one bit per block: 1 = used (64 blocks in one word) */

static int alloc_block(void)
{
    if (~bitmap == 0)
        return -1;                                 /* disk full */
    int b = __builtin_ctzl(~bitmap);               /* index of the first zero bit */
    bitmap |= 1UL << b;
    return b;
}

typedef struct { int start, len; } Extent;

typedef struct
{
    Extent ext[8];
    int n;
} File;

static void add_block(File *f, int b)
{
    if (f->n && f->ext[f->n - 1].start + f->ext[f->n - 1].len == b)
        f->ext[f->n - 1].len++;                    /* continues the last extent: just grow it */
    else
    {
        f->ext[f->n].start = b;
        f->ext[f->n].len = 1;
        f->n++;
    }
}

static int block_of(const File *f, int file_block)  /* file offset -> disk block, like the extent lookup */
{
    for (int i = 0; i < f->n; i++)
    {
        if (file_block < f->ext[i].len)
            return f->ext[i].start + file_block;
        file_block -= f->ext[i].len;
    }
    return -1;
}

static void show(const char *name, const File *f)
{
    int total = 0;
    printf("%s:", name);
    for (int i = 0; i < f->n; i++)
    {
        printf(" (start %d, len %d)", f->ext[i].start, f->ext[i].len);
        total += f->ext[i].len;
    }
    printf("  = %d blocks in %d extent%s\n", total, f->n, f->n == 1 ? "" : "s");
}

int main(void)
{
    File a = {{{0}}, 0}, b = {{{0}}, 0};
    for (int i = 0; i < 6; i++)
        add_block(&a, alloc_block());              /* file a gets blocks 0..5 in one extent */
    show("file a, written alone     ", &a);

    for (int round = 0; round < 3; round++)        /* now both files grow together and interleave */
    {
        add_block(&b, alloc_block());
        add_block(&b, alloc_block());
        add_block(&a, alloc_block());
    }
    show("file a after interleaving ", &a);
    show("file b                    ", &b);
    printf("file a, block 7 is on disk block %d; used blocks: %d of %d\n", block_of(&a, 7), __builtin_popcountl(bitmap), BLOCKS);
    return 0;
}
