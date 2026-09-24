#include <iostream>
#include <thread>
#include <atomic>

#define PUBLISHED_DATA 42
#define SEQ_CST_ITERATIONS 100000

using namespace std;

// Contrast with 04_atomics.cpp (memory_order_relaxed only): this file shows the
// full ladder of memory orderings and what each one actually buys you.
int data = 0;
atomic<bool> ready(false);

/*****************************************************************************
 * Name: producerAcqRel
 *
 * Description:
 *         Writes a plain (non-atomic) shared variable, then publishes it by
 *         storing true into the atomic "ready" flag with memory_order_release.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void producerAcqRel()
{
    ::data = PUBLISHED_DATA;
    // The release store here synchronizes-with the acquire load in
    // consumerAcqRel(): once the consumer observes ready == true via an
    // acquire load, the C++ memory model guarantees every memory write that
    // happened-before this release store (here, "data = PUBLISHED_DATA") is
    // visible to the consumer too. Release/acquire only orders around this
    // one atomic; it does not impose any global order with unrelated atomics.
    ready.store(true, memory_order_release);
}

/*****************************************************************************
 * Name: consumerAcqRel
 *
 * Description:
 *         Spins on the atomic "ready" flag using an acquire load until the
 *         producer publishes, then reads the plain shared variable.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void consumerAcqRel()
{
    while (!ready.load(memory_order_acquire))
    {
        // Busy-wait; a real program would prefer a condition variable.
    }
    cout << "acquire/release: data = " << ::data << "\n";
}

/*****************************************************************************
 * Name: producerRelaxed
 *
 * Description:
 *         Same hand-off as producerAcqRel, but using memory_order_relaxed for
 *         both the plain write's "publish" flag and its own reasoning, to
 *         illustrate the bug this creates.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void producerRelaxed()
{
    ::data = PUBLISHED_DATA;
    // BUG: memory_order_relaxed only guarantees atomicity of the store to
    // "ready" itself -- it gives no happens-before relationship with
    // "data = PUBLISHED_DATA". A relaxed store can be reordered (by the
    // compiler or, on a weakly-ordered CPU, by the hardware) ahead of the
    // plain write. On x86 the strong hardware memory model happens to keep
    // stores in program order, so this bug is easy to miss when testing only
    // on x86 -- but on ARM (weak memory model) the consumer can legitimately
    // observe ready == true while still seeing the old value of "data".
    ready.store(true, memory_order_relaxed);
}

/*****************************************************************************
 * Name: consumerRelaxed
 *
 * Description:
 *         Spins on the atomic "ready" flag using a relaxed load; paired with
 *         producerRelaxed() this is the unsafe, non-portable version of the
 *         publish pattern.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void consumerRelaxed()
{
    while (!ready.load(memory_order_relaxed))
    {
        // Busy-wait.
    }
    // On ARM this could print stale data (0) instead of PUBLISHED_DATA; on
    // x86 it will almost always print the right value, hiding the bug.
    cout << "relaxed (unsafe publish): data = " << ::data << "\n";
}

/*****************************************************************************
 * Name: bumpSeqCst
 *
 * Description:
 *         Increments a seq_cst atomic counter repeatedly. memory_order_seq_cst
 *         is the default for every std::atomic operation when no order is
 *         specified, and additionally guarantees a single global total order
 *         that all threads agree on across every seq_cst operation on every
 *         atomic variable -- not just the one being touched here. That extra
 *         guarantee is what makes it the most expensive ordering, especially
 *         on weakly-ordered hardware like ARM where it requires full memory
 *         barriers that relaxed/acquire-release do not need.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void bumpSeqCst(atomic<int> &counter)
{
    for (int i = 0; i < SEQ_CST_ITERATIONS; i = i + 1)
    {
        counter.fetch_add(1, memory_order_seq_cst);
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs the acquire/release publish pattern, the relaxed (unsafe)
 *         variant, and a seq_cst counter demo, in that order.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    thread producer1(producerAcqRel);
    thread consumer1(consumerAcqRel);
    producer1.join();
    consumer1.join();

    ready.store(false, memory_order_relaxed);
    ::data = 0;

    thread producer2(producerRelaxed);
    thread consumer2(consumerRelaxed);
    producer2.join();
    consumer2.join();

    atomic<int> seqCstCounter(0);
    thread seqCstThread1(bumpSeqCst, ref(seqCstCounter));
    thread seqCstThread2(bumpSeqCst, ref(seqCstCounter));
    seqCstThread1.join();
    seqCstThread2.join();
    cout << "seq_cst counter = " << seqCstCounter.load() << "\n";

    return 0;
}
