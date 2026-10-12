// 3 levels of 9 bits: tables allocated only on demand, translate walks the
// trie, prints tables used vs a flat table.

#include <stdio.h>
#include <stdlib.h>

#define BITS 9
#define FANOUT (1 << BITS)                        /* 512 entries per table */
#define LEVELS 3

typedef struct Table
{
    void *entry[FANOUT];                          /* next-level table, or a frame number + 1 at the last level */
} Table;

static int tables;

static Table *new_table(void)
{
    tables++;
    return calloc(1, sizeof(Table));
}

static unsigned index_at(unsigned long vpn, int level)    /* level 0 is the top */
{
    return (vpn >> (BITS * (LEVELS - 1 - level))) & (FANOUT - 1);
}

static void map(Table *root, unsigned long vpn, unsigned long frame)
{
    Table *t = root;
    for (int level = 0; level < LEVELS - 1; level++)
    {
        unsigned i = index_at(vpn, level);
        if (!t->entry[i])
            t->entry[i] = new_table();                /* allocate a table only on demand */
        t = t->entry[i];
    }
    t->entry[index_at(vpn, LEVELS - 1)] = (void *)(frame + 1);
}

static long translate(Table *root, unsigned long vpn)
{
    Table *t = root;
    for (int level = 0; level < LEVELS - 1; level++)
    {
        t = t->entry[index_at(vpn, level)];
        if (!t)
            return -1;                                /* page fault: no table here */
    }
    void *e = t->entry[index_at(vpn, LEVELS - 1)];
    return e ? (long)e - 1 : -1;
}

static void destroy(Table *t, int level)
{
    if (level < LEVELS - 1)
        for (int i = 0; i < FANOUT; i++)
            if (t->entry[i])
                destroy(t->entry[i], level + 1);
    free(t);
}

int main(void)
{
    Table *root = new_table();
    map(root, 0x0000010, 100);                        /* a code page   */
    map(root, 0x0000011, 101);                        /* the next page: shares every table */
    map(root, 0x0400020, 102);                        /* a heap page far away */
    map(root, 0x7FFFFFF, 103);                        /* a stack page at the top */

    printf("vpn 0x10      -> frame %ld\n", translate(root, 0x10));
    printf("vpn 0x11      -> frame %ld\n", translate(root, 0x11));
    printf("vpn 0x7FFFFFF -> frame %ld\n", translate(root, 0x7FFFFFF));
    printf("vpn 0x12      -> %ld (unmapped: page fault)\n", translate(root, 0x12));
    printf("vpn 0x2000000 -> %ld (no table: page fault)\n", translate(root, 0x2000000));
    printf("tables allocated: %d (%d bytes) instead of one flat table of %lu bytes\n",
           tables, (int)(tables * sizeof(Table)), (1UL << (BITS * LEVELS)) * sizeof(void *));
    destroy(root, 0);
    return 0;
}
