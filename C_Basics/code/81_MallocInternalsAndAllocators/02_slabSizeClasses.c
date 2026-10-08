#include <stddef.h>
#include <stdio.h>

// Size-class (segregated-fit) allocator: the core idea behind jemalloc,
// tcmalloc and the kernel slab/SLUB allocators. Requests round up to a class;
// each class has its own free list, so alloc/free are O(1) pointer pops/pushes
// with no searching and no coalescing. Cost: internal fragmentation.
#define NCLASSES 6
static const size_t class_size[NCLASSES] = { 16, 32, 64, 128, 256, 512 };

#define ARENA (256 * 1024)
static _Alignas(16) unsigned char arena[ARENA];
static size_t bump;                         // carve fresh objects from here

struct freeobj { struct freeobj *next; };   // free object stores its own link
static struct freeobj *freelist[NCLASSES];
static size_t requested_bytes, granted_bytes;

static int class_of(size_t n)
{
    for (int i = 0; i < NCLASSES; i++)
        if (n <= class_size[i])
            return i;
    return -1;                               // large: would go to mmap/page heap
}

static void *slab_alloc(size_t n)
{
    int c = class_of(n);
    if (c < 0)
        return NULL;
    requested_bytes += n;
    granted_bytes += class_size[c];
    struct freeobj *o = freelist[c];
    if (o)
    {
        freelist[c] = o->next;               // reuse: LIFO, cache-hot
        return o;
    }
    if (bump + class_size[c] > ARENA)
        return NULL;
    void *p = &arena[bump];
    bump += class_size[c];
    return p;
}

static void slab_free(void *p, size_t n)    // sized free, like C23 free_sized
{
    int c = class_of(n);
    struct freeobj *o = p;
    o->next = freelist[c];
    freelist[c] = o;
}

int main(void)
{
    void *a = slab_alloc(20);   // -> 32 class
    void *b = slab_alloc(33);   // -> 64 class
    printf("a=%p b=%p\n", a, b);
    slab_free(a, 20);
    void *c = slab_alloc(25);   // same class as a: reuses a's slot
    printf("c=%p (reused a: %s)\n", c, c == a ? "yes" : "no");

    for (size_t n = 1; n <= 512; n = n * 3 + 1)
        slab_alloc(n);

    printf("requested=%zu granted=%zu internal fragmentation=%.1f%%\n",
           requested_bytes, granted_bytes,
           100.0 * (granted_bytes - requested_bytes) / granted_bytes);
    return 0;
}
