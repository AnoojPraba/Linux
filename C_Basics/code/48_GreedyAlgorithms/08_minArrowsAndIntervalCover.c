// Interval greedy problems: minimum arrows to burst balloons, minimum points
// that stab all intervals, and covering a segment with the fewest intervals.

#include <stdio.h>
#include <stdlib.h>

typedef struct { int s, e; } Interval;

static int by_end(const void *a, const void *b) { return ((const Interval *)a)->e - ((const Interval *)b)->e; }
static int by_start(const void *a, const void *b) { return ((const Interval *)a)->s - ((const Interval *)b)->s; }

int main(void)
{
    Interval balloons[] = {{10, 16}, {2, 8}, {1, 6}, {7, 12}};
    int n = 4;
    qsort(balloons, n, sizeof balloons[0], by_end);       /* shoot at the earliest end, it pops every balloon spanning it */
    int arrows = 0, last = 0;
    for (int i = 0; i < n; i++)
        if (arrows == 0 || balloons[i].s > last)
        {
            arrows++;
            last = balloons[i].e;
        }
    printf("minimum arrows = %d\n", arrows);

    Interval cover[] = {{0, 3}, {2, 6}, {5, 9}, {1, 4}, {8, 12}, {3, 5}};
    int m = 6, target_start = 0, target_end = 12;
    qsort(cover, m, sizeof cover[0], by_start);
    int used = 0, reach = target_start, i = 0;
    while (reach < target_end)
    {
        int best = reach;
        while (i < m && cover[i].s <= reach)             /* among intervals starting inside the covered part, take the farthest end */
        {
            if (cover[i].e > best) best = cover[i].e;
            i++;
        }
        if (best == reach) { used = -1; break; }         /* a gap that nothing covers */
        reach = best;
        used++;
    }
    printf("fewest intervals covering [0, 12] = %d\n", used);
    return 0;
}
