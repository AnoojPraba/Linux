#include <stdio.h>

#define MAX_FRAMES 4

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

// Plain FIFO page replacement, parameterized by frame count so it can be
// run with both 3 and 4 frames below.
int fifoPageReplacement(int references[], int numReferences, int numFrames)
{
    int frames[MAX_FRAMES];
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

int main()
{
    // The classic textbook reference string that demonstrates Belady's
    // Anomaly under FIFO: 4 frames produces MORE page faults than 3
    // frames, even though more memory should intuitively never hurt.
    int references[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numReferences = sizeof(references) / sizeof(references[0]);
    int faultsWith3 = fifoPageReplacement(references, numReferences, 3);
    int faultsWith4 = fifoPageReplacement(references, numReferences, 4);

    printf("FIFO page faults with 3 frames = %d\n", faultsWith3);
    printf("FIFO page faults with 4 frames = %d\n", faultsWith4);

    if (faultsWith4 > faultsWith3)
    {
        printf("Belady's Anomaly reproduced: more frames caused more faults.\n");
    }
    else
    {
        printf("No anomaly observed for this reference string/frame counts.\n");
    }

    return 0;
}
