#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#define SETTLE_DELAY_MS 10

using namespace std;

// Deadlock demo: threadA locks mutexOne then mutexTwo; threadB locks
// mutexTwo then mutexOne. If both threads grab their first mutex before
// either grabs its second, each blocks forever waiting on the mutex the
// other thread already holds. runDeadlockProneVersion() below is written to
// make that interleaving likely, but is never actually called from main()
// because it really can hang the program - see the comment in main().
mutex mutexOne;
mutex mutexTwo;

/*****************************************************************************
 * Name: lockInOrderOneTwo
 *
 * Description:
 *         Locks mutexOne then mutexTwo, sleeping briefly in between so a
 *         concurrently running lockInOrderTwoOne has time to grab mutexTwo
 *         first, producing the classic circular-wait deadlock.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void lockInOrderOneTwo()
{
    lock_guard<mutex> lockA(mutexOne);
    this_thread::sleep_for(chrono::milliseconds(SETTLE_DELAY_MS));
    lock_guard<mutex> lockB(mutexTwo);
    cout << "lockInOrderOneTwo acquired both mutexes\n";
}

/*****************************************************************************
 * Name: lockInOrderTwoOne
 *
 * Description:
 *         Locks mutexTwo then mutexOne - the reverse order of
 *         lockInOrderOneTwo - which is what creates the deadlock when both
 *         functions run concurrently.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void lockInOrderTwoOne()
{
    lock_guard<mutex> lockB(mutexTwo);
    this_thread::sleep_for(chrono::milliseconds(SETTLE_DELAY_MS));
    lock_guard<mutex> lockA(mutexOne);
    cout << "lockInOrderTwoOne acquired both mutexes\n";
}

/*****************************************************************************
 * Name: runDeadlockProneVersion
 *
 * Description:
 *         Runs lockInOrderOneTwo and lockInOrderTwoOne concurrently. With
 *         the sleep in each function, this reliably deadlocks: never call
 *         this from a program you expect to terminate. Kept here only as a
 *         documented reference for what NOT to do; main() calls the fixed
 *         version instead.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void runDeadlockProneVersion()
{
    thread threadA(lockInOrderOneTwo);
    thread threadB(lockInOrderTwoOne);
    threadA.join();
    threadB.join();
}

/*****************************************************************************
 * Name: lockBothScoped
 *
 * Description:
 *         Acquires mutexOne and mutexTwo together via std::scoped_lock
 *         (C++17), which locks all of its arguments atomically using a
 *         deadlock-avoidance algorithm internally, so the order the
 *         arguments are listed in no longer matters across threads.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void lockBothScoped()
{
    scoped_lock bothLocks(mutexOne, mutexTwo);
    this_thread::sleep_for(chrono::milliseconds(SETTLE_DELAY_MS));
    cout << "lockBothScoped acquired both mutexes safely\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the deadlock-free fix: two threads each call
 *         lockBothScoped, which uses std::scoped_lock so the mismatched
 *         acquisition order that would deadlock plain lock_guard calls
 *         (see runDeadlockProneVersion, intentionally not invoked) can
 *         never occur.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    thread threadA(lockBothScoped);
    thread threadB(lockBothScoped);

    threadA.join();
    threadB.join();

    cout << "no deadlock: both threads finished\n";
    return 0;
}
