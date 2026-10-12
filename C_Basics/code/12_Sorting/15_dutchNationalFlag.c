// Dutch national flag: three-way partition in one pass (sort 0/1/2, partition
// around a pivot), the core of three-way quick sort.

#include <stdio.h>

static void show(const char *label, const int *a, int n)
{
    printf("%-14s:", label);
    for (int i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

static void sort012(int *a, int n)
{
    int lo = 0, mid = 0, hi = n - 1;                      /* a[0..lo) = 0s, a[lo..mid) = 1s, a[hi+1..n) = 2s */
    while (mid <= hi)
    {
        if (a[mid] == 0) swap(&a[lo++], &a[mid++]);
        else if (a[mid] == 1) mid++;
        else swap(&a[mid], &a[hi--]);                      /* the swapped-in value is unexamined: do not advance mid */
    }
}

static void partition3(int *a, int n, int pivot)
{
    int lt = 0, i = 0, gt = n - 1;
    while (i <= gt)
        if (a[i] < pivot) swap(&a[lt++], &a[i++]);
        else if (a[i] > pivot) swap(&a[i], &a[gt--]);
        else i++;
}

int main(void)
{
    int a[] = {2, 0, 2, 1, 1, 0, 2, 0, 1};
    sort012(a, 9);
    show("sort 0/1/2", a, 9);
    int b[] = {7, 3, 9, 5, 5, 1, 8, 5, 2};
    partition3(b, 9, 5);
    show("around pivot 5", b, 9);
    return 0;
}
