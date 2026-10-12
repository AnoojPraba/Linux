// Fixed-size object slots with an intrusive free list (LIFO reuse, no per-
// object header).

#include <stdio.h>
#include <stdlib.h>

#define OBJ_SIZE 32                               /* bytes per object */
#define OBJS 8                                    /* objects per slab */

typedef union Slot
{
    union Slot *next_free;                        /* when free: link to the next free slot */
    char data[OBJ_SIZE];                          /* when used: the object itself */
} Slot;

typedef struct
{
    Slot *slab;
    Slot *free_head;
    int in_use;
} Cache;

static void cache_init(Cache *c)
{
    c->slab = malloc(OBJS * sizeof(Slot));
    c->in_use = 0;
    for (int i = 0; i < OBJS - 1; i++)
        c->slab[i].next_free = &c->slab[i + 1];   /* thread the free list through the slots */
    c->slab[OBJS - 1].next_free = NULL;
    c->free_head = c->slab;
}

static void *cache_alloc(Cache *c)
{
    if (!c->free_head)
        return NULL;                              /* a real cache would add another slab */
    Slot *s = c->free_head;
    c->free_head = s->next_free;                  /* pop the head: O(1) */
    c->in_use++;
    return s;
}

static void cache_free(Cache *c, void *p)
{
    Slot *s = p;
    s->next_free = c->free_head;                  /* push: the most recently freed object is reused first */
    c->free_head = s;
    c->in_use--;
}

int main(void)
{
    Cache c;
    cache_init(&c);

    void *a = cache_alloc(&c), *b = cache_alloc(&c), *d = cache_alloc(&c);
    printf("slot indexes: a=%ld b=%ld d=%ld, in use: %d\n",
           (Slot *)a - c.slab, (Slot *)b - c.slab, (Slot *)d - c.slab, c.in_use);

    cache_free(&c, b);
    void *e = cache_alloc(&c);
    printf("freed slot %ld, next allocation got slot %ld (LIFO reuse)\n", (Slot *)b - c.slab, (Slot *)e - c.slab);

    int n = 0;
    while (cache_alloc(&c))
        n++;
    printf("allocated %d more until the slab was full; in use: %d of %d\n", n, c.in_use, OBJS);
    printf("object size %d bytes, slab size %d bytes, no per-object header\n", OBJ_SIZE, (int)(OBJS * sizeof(Slot)));
    free(c.slab);
    return 0;
}
