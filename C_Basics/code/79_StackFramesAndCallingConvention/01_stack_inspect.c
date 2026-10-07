#include <stdint.h>
#include <stdio.h>

// Observe stack layout directly. Compile with -O0 for predictable frames
// (the Makefile does not pass -O, so the default is -O0).

__attribute__((noinline)) static void show_frame(int depth)
{
    int local = depth;
    printf("depth %d: &local=%p frame=%p ret_addr=%p\n", depth, (void *)&local,
           __builtin_frame_address(0), __builtin_return_address(0));
    if (depth < 2)
        show_frame(depth + 1);
}

// Many arguments: the first few travel in registers (x0-x7 on AArch64,
// rdi/rsi/rdx/rcx/r8/r9 on x86-64 SysV), the rest are pushed on the stack.
__attribute__((noinline)) static long many_args(long a, long b, long c, long d,
                                                long e, long f, long g, long h,
                                                long i, long j)
{
    return a + b + c + d + e + f + g + h + i + j;
}

// Returning a struct: small ones come back in registers, large ones through a
// hidden pointer the caller supplies (sret).
struct big { long v[8]; };
__attribute__((noinline)) static struct big make_big(long x)
{
    struct big b;
    for (int k = 0; k < 8; k++)
        b.v[k] = x + k;
    return b;
}

__attribute__((noinline)) static char *dangling(void)
{
    char buf[16] = "gone";
    return buf;     // returns address of dead frame (UB) - gcc warns
}

int main(void)
{
    int top;
    printf("main: &top=%p\n", (void *)&top);
    show_frame(0);   // addresses DEcrease with depth: stack grows down

    printf("many_args=%ld\n", many_args(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    struct big b = make_big(100);
    printf("make_big v[0]=%ld v[7]=%ld sizeof=%zu\n", b.v[0], b.v[7], sizeof b);

    // Not dereferenced; only to show the warning-worthy pattern exists.
    (void)dangling;
    return 0;
}
