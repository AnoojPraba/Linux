#include <stdio.h>

#define ARENA_SIZE 1024
#define MIN_BLOCK_SIZE 64
#define MAX_ORDER 4
#define FREE_BLOCK 0
#define USED_BLOCK 1

// One tree node per possible block at every order (size doubles per level).
// state[order][index] tracks whether that specific block is free or used;
// a block is only actually available if it (and none of its ancestors) has
// already been split/used.
int state[MAX_ORDER + 1][ARENA_SIZE / MIN_BLOCK_SIZE];

int blockSizeAtOrder(int order)
{
    int size = MIN_BLOCK_SIZE;
    int i;

    for (i = 0; i < order; i++)
    {
        size *= 2;
    }
    return size;
}

int orderForSize(int size)
{
    int order = 0;
    int blockSize = MIN_BLOCK_SIZE;

    while ((blockSize < size) && (order < MAX_ORDER))
    {
        blockSize *= 2;
        order++;
    }
    return order;
}

// Split a free block at "order" down to "targetOrder": mark it used and
// mark both halves at the order below free, repeating until the target
// order is reached, then mark that final block used and return its index.
int splitDownTo(int order, int index, int targetOrder)
{
    if (order == targetOrder)
    {
        state[order][index] = USED_BLOCK;
        return index;
    }

    state[order][index] = USED_BLOCK;
    state[order - 1][index * 2] = FREE_BLOCK;
    state[order - 1][index * 2 + 1] = FREE_BLOCK;
    return splitDownTo(order - 1, index * 2, targetOrder);
}

// Search from the smallest workable order upward for the first free block:
// an exact-size free block is used directly, otherwise the smallest larger
// free block found is split down to the requested size.
int allocateAtOrder(int order, int targetOrder)
{
    int searchOrder;
    int index;

    for (searchOrder = targetOrder; searchOrder <= order; searchOrder++)
    {
        for (index = 0; index < (1 << (MAX_ORDER - searchOrder)); index++)
        {
            if (state[searchOrder][index] == FREE_BLOCK)
            {
                return splitDownTo(searchOrder, index, targetOrder);
            }
        }
    }
    return -1;
}

// Round the request up to the nearest power-of-2 block size, then hand back
// the offset (in bytes) of the block allocated for it, or -1 if the arena
// has no free block large enough.
int buddyAllocate(int size)
{
    int targetOrder = orderForSize(size);
    int index = allocateAtOrder(MAX_ORDER, targetOrder);

    if (index == -1)
    {
        return -1;
    }
    return index * blockSizeAtOrder(targetOrder);
}

// Mark a block free, then recursively coalesce with its buddy (the other
// half of the block it was split from) whenever that buddy is also free -
// this is what keeps the buddy system from accumulating external
// fragmentation over many allocate/free cycles.
void buddyFree(int offset, int size)
{
    int order = orderForSize(size);
    int index = offset / blockSizeAtOrder(order);

    state[order][index] = FREE_BLOCK;

    while (order < MAX_ORDER)
    {
        int buddyIndex = index ^ 1;

        if (state[order][buddyIndex] != FREE_BLOCK)
        {
            break;
        }

        state[order][index] = USED_BLOCK;
        state[order][buddyIndex] = USED_BLOCK;
        index /= 2;
        order++;
        state[order][index] = FREE_BLOCK;
    }
}

void printState()
{
    int order;
    int index;

    for (order = 0; order <= MAX_ORDER; order++)
    {
        printf("  order %d (size %4d): ", order, blockSizeAtOrder(order));
        for (index = 0; index < (1 << (MAX_ORDER - order)); index++)
        {
            printf("%d ", state[order][index]);
        }
        printf("\n");
    }
}

// All blocks start out "used" (i.e. non-existent) except the single
// whole-arena block at the top order - real free blocks only come into
// being as that top block gets split by allocateAtOrder()/splitDownTo().
void initArena()
{
    int order;
    int index;

    for (order = 0; order <= MAX_ORDER; order++)
    {
        for (index = 0; index < (1 << (MAX_ORDER - order)); index++)
        {
            state[order][index] = USED_BLOCK;
        }
    }
    state[MAX_ORDER][0] = FREE_BLOCK;
}

int main()
{
    int firstOffset;
    int secondOffset;

    initArena();

    firstOffset = buddyAllocate(100);
    printf("allocated 100 bytes at offset %d\n", firstOffset);

    secondOffset = buddyAllocate(200);
    printf("allocated 200 bytes at offset %d\n", secondOffset);

    printf("state after two allocations:\n");
    printState();

    buddyFree(firstOffset, 100);
    buddyFree(secondOffset, 200);

    printf("state after freeing both (fully coalesced back to one block):\n");
    printState();

    return 0;
}
