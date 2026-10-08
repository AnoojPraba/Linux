#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// "Implement malloc" - the classic senior question. Explicit free list over a
// fixed arena: first-fit, block splitting, and coalescing of adjacent free
// blocks on free. Header per block; payload is 16-byte aligned.
#define ARENA_SIZE (64 * 1024)
#define ALIGN 16
#define ALIGN_UP(n) (((n) + (ALIGN - 1)) & ~(size_t)(ALIGN - 1))

typedef struct block
{
    size_t size;            // payload bytes
    int free;
    struct block *next;     // next block in ADDRESS order (enables coalescing)
} block_t;

#define HDR ALIGN_UP(sizeof(block_t))

static _Alignas(ALIGN) unsigned char arena[ARENA_SIZE];
static block_t *head;

static void heap_init(void)
{
    head = (block_t *)arena;
    head->size = ARENA_SIZE - HDR;
    head->free = 1;
    head->next = NULL;
}

static void *my_malloc(size_t n)
{
    if (!head)
        heap_init();
    if (n == 0)
        return NULL;
    n = ALIGN_UP(n);
    for (block_t *b = head; b; b = b->next)
    {
        if (!b->free || b->size < n)
            continue;
        // Split if the remainder can hold a header plus a minimal payload.
        if (b->size >= n + HDR + ALIGN)
        {
            block_t *rest = (block_t *)((unsigned char *)b + HDR + n);
            rest->size = b->size - n - HDR;
            rest->free = 1;
            rest->next = b->next;
            b->size = n;
            b->next = rest;
        }
        b->free = 0;
        return (unsigned char *)b + HDR;
    }
    return NULL;            // out of memory
}

static void my_free(void *p)
{
    if (!p)
        return;
    block_t *b = (block_t *)((unsigned char *)p - HDR);
    b->free = 1;
    // Coalesce forward and backward. Singly linked list => scan for the
    // predecessor; a real allocator uses boundary tags (footers) for O(1).
    for (block_t *c = head; c; c = c->next)
        while (c->free && c->next && c->next->free)
        {
            c->size += HDR + c->next->size;
            c->next = c->next->next;
        }
}

static void dump(const char *tag)
{
    printf("-- %s\n", tag);
    for (block_t *b = head; b; b = b->next)
        printf("   %-4s size=%zu\n", b->free ? "free" : "used", b->size);
}

int main(void)
{
    void *a = my_malloc(100);
    void *b = my_malloc(200);
    void *c = my_malloc(300);
    dump("after 3 allocs");

    my_free(b);
    dump("free(b): hole in the middle (external fragmentation)");

    my_free(a);
    dump("free(a): a and b coalesce");

    my_free(c);
    dump("free(c): everything coalesces back to one block");

    void *big = my_malloc(60 * 1024);
    printf("big alloc %s\n", big ? "ok" : "failed");
    printf("alignment of a: %zu\n", (size_t)((uintptr_t)a % ALIGN));
    return 0;
}
