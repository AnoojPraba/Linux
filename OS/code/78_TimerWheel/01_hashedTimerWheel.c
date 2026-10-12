// Buckets per future tick plus a rounds counter for timers beyond one
// revolution.

#include <stdio.h>
#include <stdlib.h>

#define SLOTS 8                                   /* the wheel covers 8 ticks per revolution */

typedef struct Timer
{
    int id;
    int rounds;                                   /* full revolutions still to wait */
    struct Timer *next;
} Timer;

static Timer *wheel[SLOTS];
static int now;                                   /* current tick */

static void add_timer(int id, int delay)
{
    Timer *t = malloc(sizeof *t);
    t->id = id;
    t->rounds = (delay - 1) / SLOTS;               /* visits to skip before the one at the expiry tick */
    t->next = wheel[(now + delay) % SLOTS];
    wheel[(now + delay) % SLOTS] = t;
}

static void tick(void)
{
    now++;
    Timer **p = &wheel[now % SLOTS];
    while (*p)
    {
        Timer *t = *p;
        if (t->rounds > 0)
        {
            t->rounds--;                          /* not yet: another revolution to go */
            p = &t->next;
        }
        else
        {
            printf("tick %2d: timer %d fires\n", now, t->id);
            *p = t->next;
            free(t);
        }
    }
}

int main(void)
{
    add_timer(1, 3);
    add_timer(2, 5);
    add_timer(3, 8);                              /* exactly one revolution */
    add_timer(4, 19);                             /* two revolutions and 3 ticks */
    add_timer(5, 3);                              /* same tick as timer 1 */
    for (int i = 0; i < 20; i++)
        tick();
    return 0;
}
