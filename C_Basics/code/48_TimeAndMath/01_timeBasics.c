#include <stdio.h>
#include <time.h>

int main()
{
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    char formatted[64];
    clock_t start;
    clock_t end;
    volatile long sum = 0;
    long i;

    // time() returns seconds since the Unix epoch; localtime() breaks that
    // single number down into a struct tm (year, month, day, ...) in the
    // system's local timezone.
    printf("seconds since epoch: %ld\n", (long)now);

    // strftime formats a struct tm using the same kind of format
    // specifiers printf uses, but for dates - %Y-%m-%d %H:%M:%S here.
    strftime(formatted, sizeof(formatted), "%Y-%m-%d %H:%M:%S", localTime);
    printf("formatted local time: %s\n", formatted);

    // clock() measures CPU time consumed by this process, not wall-clock
    // time - a process that sleeps for a second uses ~0 CPU time even
    // though a second of wall-clock time passed.
    start = clock();
    for (i = 0; i < 50000000; i++)
    {
        sum += i;
    }
    end = clock();

    printf("CPU time for loop: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
