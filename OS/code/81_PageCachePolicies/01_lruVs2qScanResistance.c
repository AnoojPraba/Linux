// Replay a trace with a hot set and a long sequential scan; LRU loses the hot
// set, 2Q keeps it.

#include <stdio.h>
#include <string.h>

#define CAP 8

/* ---------- LRU: array ordered from least to most recently used ---------- */
static int lru[CAP], lru_n;

static int lru_access(int p)
{
    for (int i = 0; i < lru_n; i++)
        if (lru[i] == p)
        {
            memmove(&lru[i], &lru[i + 1], (size_t)(lru_n - i - 1) * sizeof(int));
            lru[lru_n - 1] = p;                   /* move to the most recent end */
            return 1;
        }
    if (lru_n == CAP)
    {
        memmove(&lru[0], &lru[1], (size_t)(CAP - 1) * sizeof(int));
        lru_n--;                                  /* evict the least recently used */
    }
    lru[lru_n++] = p;
    return 0;
}

/* ---------- 2Q: probation FIFO (A1) in front of a protected LRU (Am) ---------- */
#define A1_CAP 4
static int a1[A1_CAP], a1_n;
static int am[CAP - A1_CAP], am_n;

static int in(int *a, int n, int p) { for (int i = 0; i < n; i++) if (a[i] == p) return i; return -1; }

static int twoq_access(int p)
{
    int i = in(am, am_n, p);
    if (i >= 0)
    {
        memmove(&am[i], &am[i + 1], (size_t)(am_n - i - 1) * sizeof(int));
        am[am_n - 1] = p;                         /* hit in the protected queue */
        return 1;
    }
    i = in(a1, a1_n, p);
    if (i >= 0)                                   /* second reference: promote to the protected queue */
    {
        memmove(&a1[i], &a1[i + 1], (size_t)(a1_n - i - 1) * sizeof(int));
        a1_n--;
        if (am_n == CAP - A1_CAP)
        {
            memmove(&am[0], &am[1], (size_t)(am_n - 1) * sizeof(int));
            am_n--;
        }
        am[am_n++] = p;
        return 1;
    }
    if (a1_n == A1_CAP)                           /* first reference: enters probation, FIFO eviction */
    {
        memmove(&a1[0], &a1[1], (size_t)(A1_CAP - 1) * sizeof(int));
        a1_n--;
    }
    a1[a1_n++] = p;
    return 0;
}

int main(void)
{
    int trace[200], n = 0;
    for (int round = 0; round < 3; round++)        /* the hot set is touched repeatedly */
        for (int rep = 0; rep < 2; rep++)
            for (int p = 1; p <= 4; p++)
                trace[n++] = p;
    for (int p = 100; p < 130; p++)                /* one long sequential scan: never reused */
        trace[n++] = p;
    for (int rep = 0; rep < 3; rep++)              /* the hot set is used again */
        for (int p = 1; p <= 4; p++)
            trace[n++] = p;

    int lru_hits = 0, q_hits = 0, hot_after_scan_lru = 0, hot_after_scan_2q = 0;
    for (int i = 0; i < n; i++)
    {
        int h1 = lru_access(trace[i]), h2 = twoq_access(trace[i]);
        lru_hits += h1;
        q_hits += h2;
        if (i >= n - 12 && i < n - 8) { hot_after_scan_lru += h1; hot_after_scan_2q += h2; }
    }
    printf("references: %d, cache size: %d pages\n", n, CAP);
    printf("LRU : %d hits\n", lru_hits);
    printf("2Q  : %d hits\n", q_hits);
    printf("hot-set hits right after the scan (4 references): LRU %d, 2Q %d\n", hot_after_scan_lru, hot_after_scan_2q);
    return 0;
}
