#include <stdio.h>

#define NUM_BLOCKS 5
#define NUM_PROCESSES 4

// Next Fit: like first-fit, but resumes scanning from the block after the
// last one used, instead of restarting from block 0 every time - avoids
// re-scanning the same already-fragmented low blocks on every request, at
// the cost of spreading fragmentation across the whole block list instead
// of concentrating it near the start.
void nextFit(int blockSizes[], int numBlocks, int processSizes[], int numProcesses)
{
    int allocation[NUM_PROCESSES];
    int remaining[NUM_BLOCKS];
    int lastIndex = 0;
    int i;

    for (i = 0; i < numBlocks; i++)
    {
        remaining[i] = blockSizes[i];
    }

    for (i = 0; i < numProcesses; i++)
    {
        int scanned = 0;
        int index = lastIndex;

        allocation[i] = -1;
        while (scanned < numBlocks)
        {
            if (remaining[index] >= processSizes[i])
            {
                allocation[i] = index;
                remaining[index] -= processSizes[i];
                lastIndex = index;
                break;
            }
            index = (index + 1) % numBlocks;
            scanned++;
        }
    }

    printf("Next Fit:\n");
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

    nextFit(blockSizes, NUM_BLOCKS, processSizes, NUM_PROCESSES);

    return 0;
}
