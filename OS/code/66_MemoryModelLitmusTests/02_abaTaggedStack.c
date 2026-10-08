#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

// ABA in a Treiber (lock-free) stack, and the version-tag fix.
// Nodes live in a pool and are referenced by 32-bit index so index + 32-bit
// tag pack into one 64-bit word that a plain CAS can swap atomically.
#define POOL 1024
#define NIL 0xFFFFFFFFu

typedef struct { uint32_t next; int value; } node_t;
static node_t pool[POOL];

// head = (tag << 32) | index
static atomic_uint_fast64_t head_tagged = NIL;
static atomic_uint head_naive = NIL;

static uint64_t pack(uint32_t tag, uint32_t idx) { return ((uint64_t)tag << 32) | idx; }

// ---- naive: CAS on the index alone (ABA-prone) ----
static void push_naive(uint32_t n)
{
    uint32_t h = atomic_load(&head_naive);
    do pool[n].next = h;
    while (!atomic_compare_exchange_weak(&head_naive, &h, n));
}

// ---- tagged: every successful update bumps the tag ----
static void push_tagged(uint32_t n)
{
    uint64_t h = atomic_load(&head_tagged);
    do pool[n].next = (uint32_t)h;
    while (!atomic_compare_exchange_weak(&head_tagged, &h, pack((uint32_t)(h >> 32) + 1, n)));
}

static int pop_tagged(uint32_t *out)
{
    uint64_t h = atomic_load(&head_tagged);
    for (;;)
    {
        uint32_t idx = (uint32_t)h;
        if (idx == NIL)
            return 0;
        uint32_t next = pool[idx].next;
        if (atomic_compare_exchange_weak(&head_tagged, &h, pack((uint32_t)(h >> 32) + 1, next)))
        {
            *out = idx;
            return 1;
        }
    }
}

// Deterministic replay of the bad interleaving (no threads needed):
//   stack: A -> B -> C.  T1 starts pop: reads head=A, next=B, then stalls.
//   T2 pops A, pops B, pushes A back (A -> C).  B is now FREE/reused.
//   T1 resumes: CAS(head, A, B) SUCCEEDS because head is A again - but B is gone.
static void demo_aba(void)
{
    enum { A = 0, B = 1, C = 2 };
    atomic_store(&head_naive, NIL);
    push_naive(C); push_naive(B); push_naive(A);          // A -> B -> C

    uint32_t t1_head = atomic_load(&head_naive);          // T1 reads A
    uint32_t t1_next = pool[t1_head].next;                // ...and B, then stalls

    atomic_store(&head_naive, pool[A].next);              // T2 pop A
    atomic_store(&head_naive, pool[B].next);              // T2 pop B (head = C)
    pool[B].next = 0xDEAD;                                // B freed / reused
    push_naive(A);                                        // T2 push A back: A -> C

    uint32_t expect = t1_head;
    int ok = atomic_compare_exchange_strong(&head_naive, &expect, t1_next);
    printf("naive CAS succeeded: %s -> head is now %u (B, a freed node!)\n",
           ok ? "YES (ABA bug)" : "no", atomic_load(&head_naive));

    // Same schedule with tags: the tag moved on, so T1's CAS fails.
    atomic_store(&head_tagged, NIL);
    push_tagged(C); push_tagged(B); push_tagged(A);
    uint64_t th = atomic_load(&head_tagged);              // T1 snapshot (tag, A)
    uint32_t tn = pool[(uint32_t)th].next;
    uint32_t tmp;
    pop_tagged(&tmp); pop_tagged(&tmp);                   // T2 pops A, B
    push_tagged(A);                                       // T2 pushes A back
    ok = atomic_compare_exchange_strong(&head_tagged, &th, pack((uint32_t)(th >> 32) + 1, tn));
    printf("tagged CAS succeeded: %s\n", ok ? "yes (bug)" : "NO (tag changed - correct)");
}

// Concurrent stress: threads move nodes pool -> stack -> pool; nothing may be
// lost or duplicated.
static atomic_uint_fast64_t free_head = NIL;
static void free_push(uint32_t n)
{
    uint64_t h = atomic_load(&free_head);
    do pool[n].next = (uint32_t)h;
    while (!atomic_compare_exchange_weak(&free_head, &h, pack((uint32_t)(h >> 32) + 1, n)));
}
static int free_pop(uint32_t *out)
{
    uint64_t h = atomic_load(&free_head);
    for (;;)
    {
        uint32_t idx = (uint32_t)h;
        if (idx == NIL) return 0;
        if (atomic_compare_exchange_weak(&free_head, &h,
                pack((uint32_t)(h >> 32) + 1, pool[idx].next)))
        { *out = idx; return 1; }
    }
}

static void *worker(void *arg)
{
    (void)arg;
    for (int i = 0; i < 300000; i++)
    {
        uint32_t n;
        if (free_pop(&n)) push_tagged(n);
        if (pop_tagged(&n)) free_push(n);
    }
    return NULL;
}

int main(void)
{
    demo_aba();

    atomic_store(&head_tagged, NIL);
    for (uint32_t i = 0; i < POOL; i++)
        free_push(i);
    pthread_t t[4];
    for (int i = 0; i < 4; i++) pthread_create(&t[i], NULL, worker, NULL);
    for (int i = 0; i < 4; i++) pthread_join(t[i], NULL);

    int count = 0; uint32_t n;
    while (pop_tagged(&n)) { free_push(n); count++; }
    int total = 0;
    while (free_pop(&n)) total++;
    printf("stress: nodes conserved = %d (expected %d)\n", total, POOL);
    (void)count;
    return 0;
}
