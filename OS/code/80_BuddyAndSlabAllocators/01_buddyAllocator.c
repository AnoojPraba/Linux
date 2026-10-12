// Free lists per order, split on alloc, coalesce on free using buddy = page ^
// (1 << order).

#include <stdio.h>

#define MAX_ORDER 4                               /* blocks of 1, 2, 4, 8 and 16 pages */
#define PAGES (1 << MAX_ORDER)

static int free_list[MAX_ORDER + 1][PAGES];       /* page numbers of the free blocks of each order */
static int count[MAX_ORDER + 1];

static void push(int order, int page) { free_list[order][count[order]++] = page; }

static int remove_block(int order, int page)       /* remove a specific block, 1 if it was free */
{
    for (int i = 0; i < count[order]; i++)
        if (free_list[order][i] == page)
        {
            free_list[order][i] = free_list[order][--count[order]];
            return 1;
        }
    return 0;
}

static int alloc(int order)
{
    int o = order;
    while (o <= MAX_ORDER && count[o] == 0)
        o++;                                      /* find the smallest block that is big enough */
    if (o > MAX_ORDER)
        return -1;
    int page = free_list[o][--count[o]];
    while (o > order)                             /* split until the size fits */
    {
        o--;
        push(o, page + (1 << o));                 /* the upper half becomes a free buddy */
    }
    return page;
}

static void release(int page, int order)
{
    while (order < MAX_ORDER)
    {
        int buddy = page ^ (1 << order);          /* the buddy's address is one XOR away */
        if (!remove_block(order, buddy))
            break;                                /* buddy is not free: stop merging */
        page &= ~(1 << order);                    /* the merged block starts at the lower address */
        order++;
    }
    push(order, page);
}

static void show(const char *what)
{
    printf("%-22s free blocks by order 0..4:", what);
    for (int o = 0; o <= MAX_ORDER; o++)
        printf(" %d", count[o]);
    printf("\n");
}

int main(void)
{
    push(MAX_ORDER, 0);                           /* all 16 pages start as one block */
    show("start:");
    int a = alloc(0), b = alloc(1), c = alloc(0);
    printf("alloc 1 page -> page %d, 2 pages -> page %d, 1 page -> page %d\n", a, b, c);
    show("after 3 allocations:");
    release(b, 1);
    show("free the 2-page block:");
    release(a, 0);
    show("free page 0:");
    release(c, 0);
    show("free page 1 (merges):");
    return 0;
}
