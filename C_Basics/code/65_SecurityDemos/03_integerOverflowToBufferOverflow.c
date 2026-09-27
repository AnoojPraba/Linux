#include <stdio.h>
#include <stdlib.h>

// The classic pattern behind several real-world CVEs: an attacker-
// influenced count/size calculation overflows before it ever reaches
// malloc, so the "size check" the code thinks it's doing has already
// silently become wrong by the time malloc sees it.
unsigned int computeBufferSize(unsigned int elementCount, unsigned int elementSize)
{
    // On a 32-bit unsigned int, elementCount * elementSize can wrap around
    // if the true mathematical product exceeds UINT_MAX (~4.29 billion) -
    // e.g. a huge elementCount combined with a modest elementSize can
    // multiply out to something tiny after wraparound, even though the
    // caller's intent was clearly a huge buffer.
    return elementCount * elementSize;
}

int main()
{
    // Chosen so the multiplication wraps: 0x10000 * 0x10001 overflows a
    // 32-bit unsigned int and wraps down to a small number instead of the
    // huge value the caller actually asked for.
    unsigned int elementCount = 0x10000;
    unsigned int elementSize = 0x10001;
    unsigned int wrappedSize = computeBufferSize(elementCount, elementSize);
    char *buffer;
    size_t i;

    printf("requested: %u elements of %u bytes each\n", elementCount, elementSize);
    printf("naive multiplication wrapped to: %u bytes (should have been huge)\n",
           wrappedSize);

    // The code believes it allocated enough space for elementCount *
    // elementSize logical bytes - but it actually only got wrappedSize
    // bytes, which is far smaller. Any code that then writes based on the
    // *original* elementCount/elementSize (not the wrapped result) walks
    // straight off the end of this undersized buffer.
    buffer = malloc(wrappedSize);
    printf("malloc'd only %u bytes\n", wrappedSize);

    // The safe fix: check for overflow *before* multiplying (or use a
    // wider intermediate type, or a builtin like __builtin_mul_overflow),
    // never trust the multiplication's result blindly.
    if ((elementSize != 0) && (elementCount > (0xFFFFFFFFu / elementSize)))
    {
        printf("overflow check: this allocation would have overflowed - rejecting\n");
        free(buffer);
        return 1;
    }

    for (i = 0; i < wrappedSize; i++)
    {
        buffer[i] = 'A';
    }
    free(buffer);

    return 0;
}
