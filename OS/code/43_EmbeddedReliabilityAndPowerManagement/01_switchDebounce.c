#include <stdio.h>

#define STABLE_READS_REQUIRED 4
#define NUM_SAMPLES 20

// Simulated noisy raw readings from a mechanical push-button over time: the
// button is pressed (goes true), bounces a few times, settles true, is later
// released (goes false), bounces again, then settles false. A userspace demo
// can't drive a real GPIO pin, so this array stands in for successive polled
// samples of one.
static const int rawSamples[NUM_SAMPLES] =
{
    0, 0, 1, 0, 1, 1, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 0, 0, 1, 0
};

/*****************************************************************************
 * Name: debouncePoll
 *
 * Description:
 *         Feeds a sequence of raw (bouncy) samples through a counter-based
 *         debounce filter. A candidate new state must be seen for
 *         STABLE_READS_REQUIRED consecutive samples before it is accepted as
 *         the real, debounced state - any bounce back to the old value
 *         resets the counter, so a stray transition never gets through.
 *
 * Inputs:
 *         samples : array of raw boolean-ish readings taken at a fixed
 *                   polling interval.
 *         count   : number of entries in samples.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void debouncePoll(const int *samples, int count)
{
    int debouncedState = samples[0];
    int candidateState = samples[0];
    int stableCount = 0;
    int i = 0;

    printf("initial debounced state: %d\n", debouncedState);

    for (i = 0; i < count; i++)
    {
        if (samples[i] == candidateState)
        {
            stableCount++;
        }
        else
        {
            candidateState = samples[i];
            stableCount = 1;
        }

        if ((stableCount >= STABLE_READS_REQUIRED) && (candidateState != debouncedState))
        {
            debouncedState = candidateState;
            printf("sample %2d: raw = %d -> accepted debounced transition to %d\n",
                   i, samples[i], debouncedState);
        }
    }

    printf("final debounced state: %d\n", debouncedState);
}

int main()
{
    debouncePoll(rawSamples, NUM_SAMPLES);

    return 0;
}
