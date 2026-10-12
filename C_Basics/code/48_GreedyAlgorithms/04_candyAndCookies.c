// Two-pass greedy distribution problems: minimum candies for rating neighbours
// and assigning cookies to children.

#include <stdio.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

static int candies(const int *rating, int n)
{
    int give[16];
    for (int i = 0; i < n; i++) give[i] = 1;
    for (int i = 1; i < n; i++)                         /* left to right: satisfy "higher than the left neighbour" */
        if (rating[i] > rating[i - 1]) give[i] = give[i - 1] + 1;
    for (int i = n - 2; i >= 0; i--)                    /* right to left: satisfy "higher than the right neighbour" */
        if (rating[i] > rating[i + 1] && give[i] <= give[i + 1]) give[i] = give[i + 1] + 1;
    int total = 0;
    for (int i = 0; i < n; i++) total += give[i];
    return total;
}

static int assign_cookies(int *greed, int g, int *size, int s)
{
    qsort(greed, g, sizeof(int), cmp);
    qsort(size, s, sizeof(int), cmp);
    int child = 0;
    for (int cookie = 0; cookie < s && child < g; cookie++)
        if (size[cookie] >= greed[child])               /* the smallest cookie that satisfies the least greedy child */
            child++;
    return child;
}

int main(void)
{
    int r1[] = {1, 0, 2}, r2[] = {1, 2, 2}, r3[] = {1, 3, 4, 5, 2};
    printf("candies for ratings 1 0 2     : %d\n", candies(r1, 3));
    printf("candies for ratings 1 2 2     : %d\n", candies(r2, 3));
    printf("candies for ratings 1 3 4 5 2 : %d\n", candies(r3, 5));

    int greed[] = {1, 2, 3}, size[] = {1, 1};
    printf("children satisfied, greed 1 2 3 with cookies 1 1: %d\n", assign_cookies(greed, 3, size, 2));
    int greed2[] = {1, 2}, size2[] = {1, 2, 3};
    printf("children satisfied, greed 1 2 with cookies 1 2 3: %d\n", assign_cookies(greed2, 2, size2, 3));
    return 0;
}
