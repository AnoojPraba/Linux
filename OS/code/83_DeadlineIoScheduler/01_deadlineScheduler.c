// Sorted-sector sweep with an arrival-order deadline; the far request is
// pulled forward at the cost of extra head movement.

#include <stdio.h>
#include <stdlib.h>

#define N 8
#define EXPIRE 3                                   /* a request waits at most this many dispatches */

typedef struct
{
    int sector;
    int arrival;                                   /* arrival order = FIFO order */
    int done;
} Req;

int main(void)
{
    Req r[N] = {{50, 0, 0}, {52, 1, 0}, {10, 2, 0}, {54, 3, 0}, {56, 4, 0}, {58, 5, 0}, {60, 6, 0}, {62, 7, 0}};
    int head = 50, served = 0, movement = 0, far_served_at = -1;

    printf("deadline scheduler (a request may wait %d dispatches):\n", EXPIRE);
    for (int tick = 0; served < N; tick++)
    {
        int pick = -1;
        for (int i = 0; i < N && pick < 0; i++)    /* oldest request first: has it expired? */
            if (!r[i].done && tick - r[i].arrival >= EXPIRE)
                pick = i;
        const char *why = "expired, FIFO";
        if (pick < 0)                              /* otherwise the nearest sector at or above the head (a sweep) */
        {
            why = "sorted sweep";
            int best = 1 << 30;
            for (int i = 0; i < N; i++)
                if (!r[i].done && r[i].sector >= head && r[i].sector - head < best)
                {
                    best = r[i].sector - head;
                    pick = i;
                }
            if (pick < 0)                          /* nothing above the head: wrap to the lowest sector */
                for (int i = 0; i < N; i++)
                    if (!r[i].done && (pick < 0 || r[i].sector < r[pick].sector))
                        pick = i;
        }
        movement += abs(r[pick].sector - head);
        head = r[pick].sector;
        r[pick].done = 1;
        served++;
        if (head == 10)
            far_served_at = tick;
        printf("  dispatch %d: sector %2d (%s)\n", tick, head, why);
    }
    printf("total head movement = %d\n", movement);
    printf("the request for far-away sector 10 arrived at dispatch 2 and was served at dispatch %d\n", far_served_at);
    printf("a pure sweep would have served it last, at dispatch %d\n", N - 1);
    return 0;
}
