#define _GNU_SOURCE
#include <linux/futex.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

// A mutex built directly on futex(2), after Ulrich Drepper's "Futexes Are
// Tricky" (mutex2). State: 0 = unlocked, 1 = locked, 2 = locked with waiters.
// The fast path is a single atomic op in user space with NO syscall; the
// kernel is entered only under contention to sleep/wake. glibc's
// pthread_mutex_t uses this same scheme.
static long futex(atomic_int *uaddr, int op, int val)
{
    return syscall(SYS_futex, uaddr, op, val, NULL, NULL, 0);
}

typedef struct { atomic_int state; } fmutex;

static void fmutex_lock(fmutex *m)
{
    int c = 0;
    if (atomic_compare_exchange_strong(&m->state, &c, 1))
        return;                              // uncontended fast path
    // Contended: mark "locked with waiters" (2), then sleep until released.
    if (c != 2)
        c = atomic_exchange(&m->state, 2);
    while (c != 0)
    {
        futex(&m->state, FUTEX_WAIT_PRIVATE, 2);   // sleeps only if still == 2
        c = atomic_exchange(&m->state, 2);
    }
}

static void fmutex_unlock(fmutex *m)
{
    if (atomic_fetch_sub(&m->state, 1) != 1)   // there were waiters (state 2)
    {
        atomic_store(&m->state, 0);
        futex(&m->state, FUTEX_WAKE_PRIVATE, 1);   // wake one waiter
    }
}

static fmutex mtx;
static long counter;

static void *worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < 200000; i++)
    {
        fmutex_lock(&mtx);
        counter++;
        fmutex_unlock(&mtx);
    }
    return NULL;
}

int main(void)
{
    pthread_t t[4];
    for (int i = 0; i < 4; i++)
        pthread_create(&t[i], NULL, worker, NULL);
    for (int i = 0; i < 4; i++)
        pthread_join(t[i], NULL);
    printf("counter=%ld (expected %d)\n", counter, 4 * 200000);
    return 0;
}
