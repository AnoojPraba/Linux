#include <stdio.h>

#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 8

// A pool allocator for fixed-size blocks: unlike the arena above, this one
// supports freeing individual blocks, by keeping a free list threaded
// directly through the unused blocks themselves (no separate bookkeeping
// array needed) - each free block's first bytes double as a "next free
// block" pointer. This is the standard approach when a program repeatedly
// allocates/frees many same-sized objects (e.g. a fixed-size struct in a
// hot loop) and wants to avoid malloc's general-purpose overhead and
// fragmentation for that specific size.
typedef struct FreeBlock
{
    struct FreeBlock *next;
} FreeBlock;

typedef struct
{
    char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
    FreeBlock *freeList;
} Pool;

void poolInit(Pool *pool)
{
    int i;

    // Thread every block onto the free list up front, each pointing at
    // the next one - the pool starts out fully free.
    pool->freeList = (FreeBlock *)pool->storage[0];
    for (i = 0; i < POOL_BLOCK_COUNT - 1; i++)
    {
        FreeBlock *block = (FreeBlock *)pool->storage[i];

        block->next = (FreeBlock *)pool->storage[i + 1];
    }
    ((FreeBlock *)pool->storage[POOL_BLOCK_COUNT - 1])->next = NULL;
}

void *poolAlloc(Pool *pool)
{
    FreeBlock *block = pool->freeList;

    if (block == NULL)
    {
        return NULL;
    }

    // Popping the free list's head is O(1) - no scanning for a fit, since
    // every block is the same size by construction.
    pool->freeList = block->next;
    return block;
}

void poolFree(Pool *pool, void *ptr)
{
    // Freeing is just pushing this block back onto the front of the free
    // list - also O(1), and reuses the same block's memory to store the
    // "next" pointer once it's no longer holding user data.
    FreeBlock *block = (FreeBlock *)ptr;

    block->next = pool->freeList;
    pool->freeList = block;
}

int main()
{
    Pool pool;
    void *blocks[POOL_BLOCK_COUNT];
    int i;

    poolInit(&pool);

    for (i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        blocks[i] = poolAlloc(&pool);
        printf("allocated block %d at %p\n", i, blocks[i]);
    }

    printf("pool exhausted, next alloc returns: %p\n", poolAlloc(&pool));

    poolFree(&pool, blocks[3]);
    printf("freed block 3\n");

    blocks[3] = poolAlloc(&pool);
    printf("reallocated: got block 3's address back = %p\n", blocks[3]);

    return 0;
}
