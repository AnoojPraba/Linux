#include <stdio.h>

#define SKETCH_DEPTH 3
#define SKETCH_WIDTH 16
#define DJB2_SEED 5381
#define DJB2_SHIFT 5
#define SDBM_SHIFT 6
#define FNV_OFFSET_BASIS 2166136261u
#define FNV_PRIME 16777619u

typedef struct
{
    unsigned int counters[SKETCH_DEPTH][SKETCH_WIDTH];
} CountMinSketch;

/*****************************************************************************
 * Name: initSketch
 *
 * Description:
 *         Clears every counter in the sketch.
 *
 * Inputs:
 *         sketch : sketch to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void initSketch(CountMinSketch *sketch)
{
    int row;
    int col;

    for (row = 0; row < SKETCH_DEPTH; row++)
    {
        for (col = 0; col < SKETCH_WIDTH; col++)
        {
            sketch->counters[row][col] = 0;
        }
    }
}

/*****************************************************************************
 * Name: hashDjb2
 *
 * Description:
 *         Classic djb2 string hash, folded into the sketch's column range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Column index in [0, SKETCH_WIDTH).
 *****************************************************************************/
unsigned int hashDjb2(const char *item)
{
    unsigned int hash = DJB2_SEED;
    int c;

    while ((c = *item++) != '\0')
    {
        hash = ((hash << DJB2_SHIFT) + hash) + (unsigned int) c;
    }
    return hash % SKETCH_WIDTH;
}

/*****************************************************************************
 * Name: hashSdbm
 *
 * Description:
 *         Classic sdbm string hash, folded into the sketch's column range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Column index in [0, SKETCH_WIDTH).
 *****************************************************************************/
unsigned int hashSdbm(const char *item)
{
    unsigned int hash = 0;
    int c;

    while ((c = *item++) != '\0')
    {
        hash = (unsigned int) c + (hash << SDBM_SHIFT) + (hash << (SDBM_SHIFT * 2)) - hash;
    }
    return hash % SKETCH_WIDTH;
}

/*****************************************************************************
 * Name: hashFnv1a
 *
 * Description:
 *         FNV-1a string hash, folded into the sketch's column range.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *
 * Returns:
 *         Column index in [0, SKETCH_WIDTH).
 *****************************************************************************/
unsigned int hashFnv1a(const char *item)
{
    unsigned int hash = FNV_OFFSET_BASIS;
    int c;

    while ((c = *item++) != '\0')
    {
        hash ^= (unsigned int) c;
        hash *= FNV_PRIME;
    }
    return hash % SKETCH_WIDTH;
}

/*****************************************************************************
 * Name: columnForRow
 *
 * Description:
 *         Dispatches to the row-specific hash function so add()/estimate()
 *         can loop over rows uniformly.
 *
 * Inputs:
 *         item : NUL-terminated string to hash.
 *         row  : which of the D independent hash functions to use.
 *
 * Returns:
 *         Column index in [0, SKETCH_WIDTH).
 *****************************************************************************/
unsigned int columnForRow(const char *item, int row)
{
    if (row == 0)
    {
        return hashDjb2(item);
    }
    if (row == 1)
    {
        return hashSdbm(item);
    }
    return hashFnv1a(item);
}

/*****************************************************************************
 * Name: sketchAdd
 *
 * Description:
 *         Records an occurrence of an item by incrementing one counter per
 *         row, at the column that row's hash function maps the item to.
 *
 * Inputs:
 *         sketch : sketch to modify.
 *         item   : NUL-terminated string being counted.
 *
 * Returns:
 *         None
 *****************************************************************************/
void sketchAdd(CountMinSketch *sketch, const char *item)
{
    int row;

    for (row = 0; row < SKETCH_DEPTH; row++)
    {
        sketch->counters[row][columnForRow(item, row)]++;
    }
}

/*****************************************************************************
 * Name: sketchEstimate
 *
 * Description:
 *         Estimates an item's frequency as the minimum of its D counters.
 *         Hash collisions can only ever inflate a counter (another item
 *         landing in the same column adds to it), never deflate it, so
 *         taking the minimum across independent rows filters out
 *         collision noise as much as possible: the estimate can overshoot
 *         the true count but never undershoot it.
 *
 * Inputs:
 *         sketch : sketch to query.
 *         item   : NUL-terminated string being queried.
 *
 * Returns:
 *         Estimated occurrence count, guaranteed >= the true count.
 *****************************************************************************/
unsigned int sketchEstimate(const CountMinSketch *sketch, const char *item)
{
    unsigned int minCount = sketch->counters[0][columnForRow(item, 0)];
    int row;

    for (row = 1; row < SKETCH_DEPTH; row++)
    {
        unsigned int count = sketch->counters[row][columnForRow(item, row)];

        if (count < minCount)
        {
            minCount = count;
        }
    }
    return minCount;
}

int main(void)
{
    CountMinSketch sketch;
    const char *stream[] = { "get", "get", "post", "get", "delete", "get", "post" };
    int streamLen = sizeof(stream) / sizeof(stream[0]);
    int i;

    initSketch(&sketch);
    for (i = 0; i < streamLen; i++)
    {
        sketchAdd(&sketch, stream[i]);
    }

    printf("estimate(get) = %u (true count 4)\n", sketchEstimate(&sketch, "get"));
    printf("estimate(post) = %u (true count 2)\n", sketchEstimate(&sketch, "post"));
    printf("estimate(delete) = %u (true count 1)\n", sketchEstimate(&sketch, "delete"));
    printf("estimate(put) = %u (true count 0, may overestimate)\n",
           sketchEstimate(&sketch, "put"));

    return 0;
}
