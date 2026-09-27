#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DEMO_BUFFER_LEN 11
#define OVERLAP_COPY_LEN 5

// Hand-rolled memmove(): unlike memcpy, correctly handles overlapping
// src/dest regions by detecting the overlap direction and copying forward
// or backward accordingly. See NOTES.md for why memcpy is undefined on
// overlap and exactly how the direction check avoids it.

/*****************************************************************************
 * Name: my_memmove
 *
 * Description:
 *         Copies n bytes from src to dest, correctly handling the case
 *         where the source and destination regions overlap.
 *
 * Inputs:
 *         dest : destination buffer.
 *         src  : source buffer.
 *         n    : number of bytes to copy.
 *
 * Returns:
 *         dest.
 *****************************************************************************/
void *my_memmove(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *) dest;
    const unsigned char *s = (const unsigned char *) src;
    size_t i;

    // Real-world implementations (e.g. glibc) also copy word-at-a-time
    // (uint64_t-sized chunks) when both pointers share the same alignment,
    // for a large speed win over this byte-by-byte version - omitted here
    // to keep the overlap-direction logic, which is the essential point,
    // as clear as possible.
    if (d < s)
    {
        // dest starts before src: copying forward (low to high addresses)
        // is safe, since we always read a byte before any later write could
        // reach that same address.
        for (i = 0; i < n; i++)
        {
            d[i] = s[i];
        }
    }
    else if (d > s)
    {
        // dest starts after src: copying forward would overwrite source
        // bytes before they've been read (whenever the regions overlap), so
        // copy backward (high to low addresses) instead.
        for (i = n; i > 0; i--)
        {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

int main(void)
{
    char buffer[DEMO_BUFFER_LEN] = "0123456789";

    // Overlapping shift-right: dest > src, must copy backward.
    my_memmove(buffer + 2, buffer, OVERLAP_COPY_LEN);
    printf("after shift-right: %s\n", buffer);

    strcpy(buffer, "0123456789");
    // Overlapping shift-left: dest < src, safe to copy forward.
    my_memmove(buffer, buffer + 2, OVERLAP_COPY_LEN);
    printf("after shift-left:  %s\n", buffer);

    return 0;
}
