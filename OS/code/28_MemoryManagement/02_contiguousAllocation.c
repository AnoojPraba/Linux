#include <stdio.h>

#define NUM_BLOCKS 5
#define NUM_PROCESSES 4

// First Fit: allocate the first free block large enough to hold the
// request - fast (stops at the first match), but can leave many small
// unusable gaps (external fragmentation) scattered throughout memory.
void firstFit(int blockSizes[], int numBlocks, int processSizes[], int numProcesses)
{
    int allocation[NUM_PROCESSES];
    int remaining[NUM_BLOCKS];
    int i;
    int j;

    for (i = 0; i < numBlocks; i++)
    {
        remaining[i] = blockSizes[i];
    }

    for (i = 0; i < numProcesses; i++)
    {
        allocation[i] = -1;
        for (j = 0; j < numBlocks; j++)
        {
            if (remaining[j] >= processSizes[i])
            {
                allocation[i] = j;
                remaining[j] -= processSizes[i];
                break;
            }
        }
    }

    printf("First Fit:\n");
    for (i = 0; i < numProcesses; i++)
    {
        if (allocation[i] != -1)
        {
            printf("  process %d (size %d) -> block %d\n", i, processSizes[i],
                   allocation[i]);
        }
        else
        {
            printf("  process %d (size %d) -> not allocated\n", i, processSizes[i]);
        }
    }
}

// Best Fit: allocate the smallest free block that's still large enough -
// minimizes the leftover gap for this allocation, at the cost of scanning
// every block instead of stopping at the first match, and still leaves
// small, likely-unusable leftover fragments over time.
void bestFit(int blockSizes[], int numBlocks, int processSizes[], int numProcesses)
{
    int allocation[NUM_PROCESSES];
    int remaining[NUM_BLOCKS];
    int i;
    int j;

    for (i = 0; i < numBlocks; i++)
    {
        remaining[i] = blockSizes[i];
    }

    for (i = 0; i < numProcesses; i++)
    {
        int bestIndex = -1;

        for (j = 0; j < numBlocks; j++)
        {
            if (remaining[j] >= processSizes[i])
            {
                if ((bestIndex == -1) || (remaining[j] < remaining[bestIndex]))
                {
                    bestIndex = j;
                }
            }
        }

        allocation[i] = bestIndex;
        if (bestIndex != -1)
        {
            remaining[bestIndex] -= processSizes[i];
        }
    }

    printf("Best Fit:\n");
    for (i = 0; i < numProcesses; i++)
    {
        if (allocation[i] != -1)
        {
            printf("  process %d (size %d) -> block %d\n", i, processSizes[i],
                   allocation[i]);
        }
        else
        {
            printf("  process %d (size %d) -> not allocated\n", i, processSizes[i]);
        }
    }
}

int main()
{
    int blockSizes[NUM_BLOCKS] = {100, 500, 200, 300, 600};
    int processSizes[NUM_PROCESSES] = {212, 417, 112, 426};

    firstFit(blockSizes, NUM_BLOCKS, processSizes, NUM_PROCESSES);
    bestFit(blockSizes, NUM_BLOCKS, processSizes, NUM_PROCESSES);

    return 0;
}
