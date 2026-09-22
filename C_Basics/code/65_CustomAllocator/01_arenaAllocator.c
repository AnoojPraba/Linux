#include <stdio.h>

#define ARENA_SIZE 1024

// An arena (bump) allocator: one big block obtained once (a static buffer
// here; a real one would typically mmap it), handed out in slices by just
// advancing an offset - no per-allocation bookkeeping, no free(). This is
// the fastest possible allocator (a single pointer add), at the cost of
// only being able to free everything at once via arenaReset(), never a
// single allocation individually - the right tool when a batch of
// allocations all share the same lifetime (e.g. everything needed for one
// request, one frame of a game loop, one parse pass).
typedef struct
{
    char buffer[ARENA_SIZE];
    size_t offset;
} Arena;

void arenaInit(Arena *arena)
{
    arena->offset = 0;
}

void *arenaAlloc(Arena *arena, size_t size)
{
    // Round the offset up to 8-byte alignment before handing it out -
    // without this, a request for e.g. 3 bytes would leave the next
    // allocation starting on an odd address, which is invalid for types
    // like double that require aligned access.
    size_t alignedOffset = (arena->offset + 7) & ~((size_t)7);

    if (alignedOffset + size > ARENA_SIZE)
    {
        return NULL;
    }

    arena->offset = alignedOffset + size;
    return &arena->buffer[alignedOffset];
}

void arenaReset(Arena *arena)
{
    arena->offset = 0;
}

int main()
{
    Arena arena;
    int *numbers;
    double *values;
    int i;

    arenaInit(&arena);

    numbers = arenaAlloc(&arena, 5 * sizeof(int));
    for (i = 0; i < 5; i++)
    {
        numbers[i] = i * i;
    }
    printf("numbers: ");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    values = arenaAlloc(&arena, 3 * sizeof(double));
    for (i = 0; i < 3; i++)
    {
        values[i] = i * 1.5;
    }
    printf("values: %f %f %f\n", values[0], values[1], values[2]);

    printf("arena bytes used = %zu\n", arena.offset);

    // One call frees everything allocated since the last reset - no
    // individual free() for "numbers" or "values" needed or possible.
    arenaReset(&arena);
    printf("arena bytes used after reset = %zu\n", arena.offset);

    return 0;
}
