#include <stdio.h>

#define NUM_FRAMES 3
#define EMPTY_PAGE (-1)

// Second-Chance / Clock: frames form a circular buffer with a "hand". Each
// resident page has a reference bit, set whenever it's accessed. On a
// fault, the hand advances past frames whose reference bit is set
// (clearing the bit and giving that page a "second chance" instead of
// evicting it) until it finds one with the bit clear, which it evicts -
// a cheap hardware-friendly approximation of true LRU.
int clockPageReplacement(int references[], int numReferences, int numFrames)
{
    int frames[NUM_FRAMES];
    int referenceBit[NUM_FRAMES];
    int hand = 0;
    int pageFaults = 0;
    int i;
    int f;

    for (i = 0; i < numFrames; i++)
    {
        frames[i] = EMPTY_PAGE;
        referenceBit[i] = 0;
    }

    for (i = 0; i < numReferences; i++)
    {
        int page = references[i];
        int found = 0;

        for (f = 0; f < numFrames; f++)
        {
            if (frames[f] == page)
            {
                referenceBit[f] = 1;
                found = 1;
                break;
            }
        }

        if (found)
        {
            continue;
        }

        pageFaults++;
        while (referenceBit[hand] == 1)
        {
            referenceBit[hand] = 0;
            hand = (hand + 1) % numFrames;
        }
        frames[hand] = page;
        referenceBit[hand] = 1;
        hand = (hand + 1) % numFrames;
    }

    return pageFaults;
}

int main()
{
    int references[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numReferences = sizeof(references) / sizeof(references[0]);

    printf("Clock page faults (3 frames) = %d\n",
           clockPageReplacement(references, numReferences, NUM_FRAMES));

    return 0;
}
