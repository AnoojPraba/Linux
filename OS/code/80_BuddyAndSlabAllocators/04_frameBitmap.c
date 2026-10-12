// One bit per frame: allocate, free, count with bit operations.

#include <stdio.h>

#define FRAMES 16

static unsigned char bitmap[FRAMES / 8];        /* 16 frames = 2 bytes */

static int test(int f)  { return bitmap[f / 8] >> (f % 8) & 1; }
static void set(int f)  { bitmap[f / 8] |= (unsigned char)(1u << (f % 8)); }
static void clear(int f){ bitmap[f / 8] &= (unsigned char)~(1u << (f % 8)); }

static int alloc_frame(void)
{
    for (int f = 0; f < FRAMES; f++)
        if (!test(f))
        {
            set(f);
            return f;
        }
    return -1;                                  /* out of memory */
}

static int free_count(void)
{
    int c = 0;
    for (int f = 0; f < FRAMES; f++)
        c += !test(f);
    return c;
}

int main(void)
{
    set(0); set(1);                             /* frames 0 and 1 hold the kernel */
    printf("free frames: %d\n", free_count());

    int a = alloc_frame(), b = alloc_frame(), c = alloc_frame();
    printf("allocated frames %d, %d, %d; free: %d\n", a, b, c, free_count());

    clear(b);                                   /* give frame b back */
    printf("freed frame %d; next allocation gets frame %d\n", b, alloc_frame());
    return 0;
}
