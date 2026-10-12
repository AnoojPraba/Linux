// Searching a sorted sequence of unknown length: exponential (galloping)
// search over a generator function.

#include <functional>
#include <iostream>

long probes;

// the "sequence": a monotone function we can evaluate at any index but whose length we do not know
long element_at(long i) { ++probes; return i * i + 3 * i; }

long search(const std::function<long(long)> &at, long key)
{
    long bound = 1;
    while (at(bound) < key) bound *= 2;                     // gallop: 1, 2, 4, 8, ... until we pass the key
    long lo = bound / 2, hi = bound;                        // the key, if present, is in (bound/2, bound]
    while (lo <= hi)
    {
        long mid = lo + (hi - lo) / 2, v = at(mid);
        if (v == key) return mid;
        if (v < key) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}

int main()
{
    for (long key : {40L, 1000003L, 123456L * 123456L + 3 * 123456L, 41L})
    {
        probes = 0;
        long idx = search(element_at, key);
        std::cout << "key " << key << " -> index " << idx << " after " << probes << " probes\n";
    }
    std::cout << "O(log i) where i is the answer's position, so it works when the length is unknown or huge (and finds near-front keys fast)\n";
}
