#include <stdio.h>

#define NUM_FRAMES 3
#define MAX_REFERENCES 20

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

// FIFO: evict whichever page has been in a frame the longest, regardless
// of how recently or often it was actually used - simple to implement (a
// single rotating index), but can evict a page that's about to be needed
// again right after being replaced.
int fifoPageReplacement(int references[], int numReferences, int numFrames)
{
    int frames[NUM_FRAMES];
    int count = 0;
    int replaceIndex = 0;
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
            frames[replaceIndex] = page;
            replaceIndex = (replaceIndex + 1) % numFrames;
        }
    }

    return pageFaults;
}

// LRU: evict whichever page in memory was used furthest in the past -
// this needs to track recency per page, unlike FIFO's simple rotation, and
// usually approximates "keep what's probably needed again soon" well. It
// is not guaranteed to beat FIFO on every reference string, though - the
// two can be compared directly below, and this particular reference
// string happens to give FIFO one fewer fault than LRU.
int lruPageReplacement(int references[], int numReferences, int numFrames)
{
    int frames[NUM_FRAMES];
    int lastUsed[NUM_FRAMES];
    int count = 0;
    int pageFaults = 0;
    int i;
    int time;

    for (i = 0; i < numFrames; i++)
    {
        frames[i] = -1;
    }

    for (time = 0; time < numReferences; time++)
    {
        int page = references[time];
        int index = -1;

        for (i = 0; i < count; i++)
        {
            if (frames[i] == page)
            {
                index = i;
                break;
            }
        }

        if (index != -1)
        {
            lastUsed[index] = time;
            continue;
        }

        pageFaults++;
        if (count < numFrames)
        {
            frames[count] = page;
            lastUsed[count] = time;
            count++;
        }
        else
        {
            int lruIndex = 0;

            for (i = 1; i < numFrames; i++)
            {
                if (lastUsed[i] < lastUsed[lruIndex])
                {
                    lruIndex = i;
                }
            }
            frames[lruIndex] = page;
            lastUsed[lruIndex] = time;
        }
    }

    return pageFaults;
}

int main()
{
    int references[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numReferences = sizeof(references) / sizeof(references[0]);

    printf("FIFO page faults (3 frames) = %d\n",
           fifoPageReplacement(references, numReferences, NUM_FRAMES));
    printf("LRU page faults (3 frames)  = %d\n",
           lruPageReplacement(references, numReferences, NUM_FRAMES));

    return 0;
}
