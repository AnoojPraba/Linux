#include <iostream>
#include <mutex>

#define COUNTDOWN_START 3

using namespace std;

// std::recursive_mutex lets the SAME thread lock it multiple times without
// deadlocking itself (each lock() must be matched by an unlock(), which
// lock_guard handles automatically). A plain std::mutex would deadlock the
// thread on the second, nested lock() call below.
//
// This is usually a code smell: needing recursive locking often means the
// locking and the recursive logic should be split into a locked public
// entry point plus an unlocked private helper that the recursive calls use,
// rather than reaching for std::recursive_mutex.
recursive_mutex countdownMutex;

/*****************************************************************************
 * Name: countdown
 *
 * Description:
 *         Recursively locks countdownMutex on every call, including the
 *         recursive calls made while still holding the lock, then prints
 *         the current count before unwinding.
 *
 * Inputs:
 *         count : the current countdown value.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void countdown(int count)
{
    lock_guard<recursive_mutex> guard(countdownMutex);

    cout << "countdown " << count << "\n";
    if (count > 0)
    {
        countdown(count - 1);
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls the recursive countdown function to show a single thread
 *         re-acquiring a recursive_mutex it already holds.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    countdown(COUNTDOWN_START);
    return 0;
}
