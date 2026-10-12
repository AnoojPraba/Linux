// Calendar time round trip: gmtime, strftime, out-of-range fields normalized by
// timegm, difftime, and CPU time with clock().

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t t = 1700000000;                      /* seconds since 1970-01-01 00:00:00 UTC */
    struct tm *utc = gmtime(&t);                /* broken-down time in UTC */

    char buf[64];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", utc);
    printf("ISO style : %s\n", buf);
    strftime(buf, sizeof buf, "%A, %d %B %Y, %I:%M %p", utc);
    printf("long style: %s\n", buf);
    printf("weekday=%d day-of-year=%d\n", utc->tm_wday, utc->tm_yday);

    /* build a time from fields and compute a difference */
    struct tm later = *utc;
    later.tm_mday += 10;                        /* may overflow the month: normalized by timegm */
    time_t t2 = timegm(&later);
    printf("10 days later: %.0f seconds, date is now %04d-%02d-%02d\n",
           difftime(t2, t), later.tm_year + 1900, later.tm_mon + 1, later.tm_mday);

    /* CPU time used by a block of work */
    clock_t c0 = clock();
    volatile unsigned long x = 0;
    for (unsigned long i = 0; i < 2000000UL; i++)
        x += i;
    double cpu = (double)(clock() - c0) / CLOCKS_PER_SEC;
    printf("cpu time measured: %s\n", cpu >= 0.0 ? "ok" : "bad");
    return 0;
}
