#include <iostream>

using namespace std;

#define EXPENSIVE_COMPUTE_RESULT 1764

// Demonstrates mutable: a const method may need to modify a member for
// caching or logging purposes even though it doesn't change the object's
// externally-visible state (logical constness vs bitwise constness).
class ExpensiveCalculator
{
    private:
        int input;

        // mutable: allowed to change even from a const member function,
        // because it's implementation-detail bookkeeping (a cache and an
        // access counter), not part of the object's logical/observable
        // state. Without "mutable" here, assigning to either field inside
        // compute() (a const method) would fail to compile.
        mutable int cachedResult;
        mutable bool hasCachedResult;
        mutable int accessCount;

    public:
        // Trivial constructor with initializer list.
        explicit ExpensiveCalculator(int value)
            : input(value), cachedResult(0), hasCachedResult(false), accessCount(0)
        {
        }

        /*****************************************************************************
         * Name: compute
         *
         * Description:
         *         Returns the (simulated) expensive computation over input,
         *         computing it only once and caching the result for later
         *         calls. Also bumps an access counter every call. Both the
         *         cache and the counter are logging/caching implementation
         *         details -- the method is logically const because the
         *         externally-visible result never changes between calls.
         *
         * Returns:
         *         The computed result for this object's input.
         *****************************************************************************/
        int compute() const
        {
            accessCount += 1;
            if (!hasCachedResult)
            {
                cout << "computing (expensive) for input " << input << "\n";
                cachedResult = EXPENSIVE_COMPUTE_RESULT;
                hasCachedResult = true;
            }
            else
            {
                cout << "returning cached result\n";
            }
            return cachedResult;
        }

        // Trivial getter.
        int getAccessCount() const { return accessCount; }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls compute() on a const ExpensiveCalculator twice, showing the
 *         first call computes and caches, the second call reuses the cache,
 *         and the access counter increments both times -- all via a const
 *         object and const member function.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    const ExpensiveCalculator calculator(42);

    cout << "result: " << calculator.compute() << "\n";
    cout << "result: " << calculator.compute() << "\n";
    cout << "access count: " << calculator.getAccessCount() << "\n";
    return 0;
}
