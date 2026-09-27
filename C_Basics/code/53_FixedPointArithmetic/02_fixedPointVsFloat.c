#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define Q16_16_FRAC_BITS 16
#define TIMING_LOOP_ITERATIONS 20000000
#define Q16_16_APPROX_MAX_RANGE 32768

/*****************************************************************************
 * Name: timeFixedPointLoop
 *
 * Description:
 *         Sums a fixed number of Q16.16 fixed-point increments using plain
 *         integer addition and reports the CPU time taken.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void timeFixedPointLoop(void)
{
    int32_t sum = 0;
    int32_t step = 1 << Q16_16_FRAC_BITS;
    clock_t start = clock();
    long i;

    for (i = 0; i < TIMING_LOOP_ITERATIONS; i++)
    {
        sum += step;
    }

    printf("fixed-point loop: %f seconds (sum = %d)\n",
           (double)(clock() - start) / CLOCKS_PER_SEC, sum);
}

/*****************************************************************************
 * Name: timeFloatLoop
 *
 * Description:
 *         Sums the same number of floating-point increments and reports the
 *         CPU time taken, for comparison against the fixed-point loop.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void timeFloatLoop(void)
{
    double sum = 0.0;
    clock_t start = clock();
    long i;

    for (i = 0; i < TIMING_LOOP_ITERATIONS; i++)
    {
        sum += 1.0;
    }

    printf("floating-point loop: %f seconds (sum = %f)\n",
           (double)(clock() - start) / CLOCKS_PER_SEC, sum);
}

int main(void)
{
    // On an FPU-less MCU, every "float" operation above is actually a call
    // into a software floating-point emulation library (soft-float), which
    // takes a variable, much larger number of cycles per operation than the
    // plain integer add/sub/mul used for fixed-point. Fixed-point math runs
    // on ordinary integer ALU instructions, so its execution time per
    // operation is fixed and predictable - a property real-time control
    // loops depend on. On a desktop CPU with a hardware FPU (like the one
    // running this demo) both loops use real machine instructions, so the
    // timing difference here will be much smaller than on an FPU-less MCU.
    timeFixedPointLoop();
    timeFloatLoop();

    // The tradeoff: Q16.16 only represents roughly +-32768 with about
    // 2^-16 (~0.0000153) precision - both range and precision are fixed at
    // compile time by the chosen Q-format. Floating point instead trades a
    // fixed precision-per-value for a huge dynamic range, spending some of
    // its bits on a floating exponent so it can represent both tiny and
    // huge magnitudes, at the cost of that soft-float overhead when no FPU
    // is present.
    printf("Q16.16 usable range is approximately +-%d\n", Q16_16_APPROX_MAX_RANGE);

    return 0;
}
