// Binary search on the answer with a monotone predicate: integer sqrt, minimum
// eating speed, ship capacity and a generic helper.

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

// smallest x in [lo, hi] with ok(x) true, assuming ok is monotone (false...false true...true)
long first_true(long lo, long hi, const std::function<bool(long)> &ok)
{
    while (lo < hi)
    {
        long mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid; else lo = mid + 1;
    }
    return lo;
}

int main()
{
    long x = 2147395600;
    long root = first_true(0, 46341, [&](long m) { return m * m > x; }) - 1;           // largest m with m*m <= x
    std::cout << "isqrt(" << x << ") = " << root << '\n';

    std::vector<int> piles{3, 6, 7, 11};
    auto hours = [&](long speed) { long h = 0; for (int p : piles) h += (p + speed - 1) / speed; return h; };
    std::cout << "minimum eating speed for 8 hours: " << first_true(1, *std::max_element(piles.begin(), piles.end()), [&](long s) { return hours(s) <= 8; }) << '\n';

    std::vector<int> w{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto days = [&](long cap) { int d = 1; long load = 0; for (int x : w) { if (load + x > cap) { ++d; load = 0; } load += x; } return d; };
    long lo = *std::max_element(w.begin(), w.end()), hi = std::accumulate(w.begin(), w.end(), 0L);
    std::cout << "minimum ship capacity for 5 days: " << first_true(lo, hi, [&](long c) { return days(c) <= 5; }) << '\n';

    std::vector<int> a{7, 2, 5, 10, 8};                                                  // split into 2 subarrays minimizing the largest sum
    auto parts = [&](long limit) { int p = 1; long s = 0; for (int v : a) { if (s + v > limit) { ++p; s = 0; } s += v; } return p; };
    std::cout << "smallest possible largest sum when splitting {7,2,5,10,8} in 2: "
              << first_true(*std::max_element(a.begin(), a.end()), std::accumulate(a.begin(), a.end(), 0L), [&](long l) { return parts(l) <= 2; }) << '\n';
}
