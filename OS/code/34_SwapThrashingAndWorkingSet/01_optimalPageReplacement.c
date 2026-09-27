#include <stdio.h>

#define NUM_FRAMES 3

int contains(int frames[], int count, int page)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (frames[i] == page)
        {
            return 1;
        }
    }
    return 0;
}

// Optimal (Belady's algorithm): evict whichever resident page will not be
// used again for the longest time in the future, looking ahead through the
// rest of the reference string. This needs full foreknowledge of future
// accesses, so it's not usable online - it exists as the provable
// minimum-fault benchmark that real algorithms (FIFO/LRU) are measured
// against.
int optimalPageReplacement(int references[], int numReferences, int numFrames)
{
    int frames[NUM_FRAMES];
    int count = 0;
    int pageFaults = 0;
    int i;

    for (i = 0; i < numReferences; i++)
    {
        int page = references[i];

        if (contains(frames, count, page))
        {
            continue;
        }

        pageFaults++;
        if (count < numFrames)
        {
            frames[count++] = page;
        }
        else
        {
            int farthestIndex = 0;
            int farthestDistance = -1;
            int f;

            for (f = 0; f < numFrames; f++)
            {
                int j;
                int distance = numReferences;

                for (j = i + 1; j < numReferences; j++)
                {
                    if (references[j] == frames[f])
                    {
                        distance = j;
                        break;
                    }
                }

                if (distance > farthestDistance)
                {
                    farthestDistance = distance;
                    farthestIndex = f;
                }
            }
            frames[farthestIndex] = page;
        }
    }

    return pageFaults;
}

int main()
{
    int references[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numReferences = sizeof(references) / sizeof(references[0]);

    printf("Optimal page faults (3 frames) = %d\n",
           optimalPageReplacement(references, numReferences, NUM_FRAMES));

    return 0;
}
