#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

// Litmus tests: run a tiny two-thread pattern millions of times and count
// outcomes the memory model says are (or are not) allowed.
//   MP (message passing): T0: data=1; flag=1   T1: r1=flag; r2=data
//        forbidden with release/acquire: r1==1 && r2==0
//   SB (store buffering): T0: x=1; r1=y        T1: y=1; r2=x
//        forbidden only with seq_cst: r1==0 && r2==0
// x86 (TSO) reorders only store->load, so MP never fails there; ARM/POWER
// (this Pi is AArch64) can reorder everything unless ordered.
//
// RESULT ON THIS PI (Cortex-A76, 4 cores): all five rows print 0 even for the
// "allowed" outcomes - the hardware is stronger in practice than the model, or
// the race window is below this harness's resolution. A 0 is NOT proof the
// code is correct; only the "must be 0" rows are guaranteed by the standard.
// Tools that do observe weak behavior: litmus7 (diy7), herd7 (model checker),
// or the same harness on other ARM/POWER cores.
//
// Harness notes (why the numbers are only visible with this setup):
//  * a third coordinator thread resets the cells and releases both workers by
//    bumping one shared word, so they start within a few ns of each other;
//  * the cells live on separate cache lines and were just written by the
//    coordinator, so each worker's first access is a cross-core cache miss.
//    A store then sits in the store buffer while a later load completes.
#define N 1000000
#define K 4096                          // cells reused round-robin

struct cell { _Alignas(64) atomic_int v; };
static struct cell data[K], flag[K], x[K], y[K];
static int r1[K], r2[K];
static atomic_int go, done;
static memory_order mo_store, mo_load;
static int mode;                        // 0 = MP, 1 = SB

static void *t0(void *a)
{
    (void)a;
    for (int i = 0; i < N; i++)
    {
        int k = i % K;
        while (atomic_load_explicit(&go, memory_order_acquire) <= i)
            ;
        if (mode == 0)
        {
            atomic_store_explicit(&data[k].v, 1, memory_order_relaxed);
            atomic_store_explicit(&flag[k].v, 1, mo_store);
        }
        else
        {
            atomic_store_explicit(&x[k].v, 1, mo_store);
            r1[k] = atomic_load_explicit(&y[k].v, mo_load);
        }
        atomic_fetch_add_explicit(&done, 1, memory_order_release);
    }
    return NULL;
}

static void *t1(void *a)
{
    (void)a;
    for (int i = 0; i < N; i++)
    {
        int k = i % K;
        while (atomic_load_explicit(&go, memory_order_acquire) <= i)
            ;
        if (mode == 0)
        {
            r1[k] = atomic_load_explicit(&flag[k].v, mo_load);
            r2[k] = atomic_load_explicit(&data[k].v, memory_order_relaxed);
        }
        else
        {
            atomic_store_explicit(&y[k].v, 1, mo_store);
            r2[k] = atomic_load_explicit(&x[k].v, mo_load);
        }
        atomic_fetch_add_explicit(&done, 1, memory_order_release);
    }
    return NULL;
}

static long run(int m, memory_order st, memory_order ld)
{
    mode = m; mo_store = st; mo_load = ld;
    atomic_store(&go, 0);
    atomic_store(&done, 0);
    pthread_t a, b;
    pthread_create(&a, NULL, t0, NULL);
    pthread_create(&b, NULL, t1, NULL);

    long bad = 0;
    for (int i = 0; i < N; i++)
    {
        int k = i % K;
        atomic_store_explicit(&data[k].v, 0, memory_order_relaxed);
        atomic_store_explicit(&flag[k].v, 0, memory_order_relaxed);
        atomic_store_explicit(&x[k].v, 0, memory_order_relaxed);
        atomic_store_explicit(&y[k].v, 0, memory_order_relaxed);
        atomic_store_explicit(&go, i + 1, memory_order_release);   // release both
        while (atomic_load_explicit(&done, memory_order_acquire) < 2 * (i + 1))
            ;
        bad += (m == 0) ? (r1[k] == 1 && r2[k] == 0) : (r1[k] == 0 && r2[k] == 0);
    }
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    return bad;
}

int main(void)
{
    printf("MP bad outcome (flag seen, data stale), %d runs:\n", N);
    printf("  relaxed store / relaxed load : %ld   (allowed by the model)\n",
           run(0, memory_order_relaxed, memory_order_relaxed));
    printf("  release store / acquire load : %ld   (must be 0)\n",
           run(0, memory_order_release, memory_order_acquire));
    printf("SB bad outcome (both read 0), %d runs:\n", N);
    printf("  relaxed                      : %ld   (allowed by the model)\n",
           run(1, memory_order_relaxed, memory_order_relaxed));
    printf("  release/acquire              : %ld   (allowed by the model; see RESULT note)\n",
           run(1, memory_order_release, memory_order_acquire));
    printf("  seq_cst                      : %ld   (must be 0)\n",
           run(1, memory_order_seq_cst, memory_order_seq_cst));
    return 0;
}
