// Fast power mod, inversion counting, maximum subarray by merging four-number
// summaries.

#include <algorithm>
#include <iostream>
#include <vector>

using u64 = unsigned long long;

u64 power_mod(u64 base, u64 exp, u64 mod)               // O(log exp)
{
    u64 result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long merge_count(std::vector<int> &a, std::vector<int> &tmp, int lo, int hi)    // sorts a[lo, hi), returns the inversions
{
    if (hi - lo < 2) return 0;
    int mid = lo + (hi - lo) / 2;
    long inv = merge_count(a, tmp, lo, mid) + merge_count(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
    {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else { tmp[k++] = a[j++]; inv += mid - i; }       // a[j] is smaller than every remaining left element
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    std::copy(tmp.begin() + lo, tmp.begin() + hi, a.begin() + lo);
    return inv;
}

struct Seg { long sum, best, prefix, suffix; };         // what the combine step needs from each half

Seg max_subarray(const std::vector<int> &a, int lo, int hi)    // a[lo, hi)
{
    if (hi - lo == 1) { long x = a[lo]; return {x, x, x, x}; }
    int mid = lo + (hi - lo) / 2;
    Seg l = max_subarray(a, lo, mid), r = max_subarray(a, mid, hi);
    return {l.sum + r.sum,
            std::max({l.best, r.best, l.suffix + r.prefix}),    // the best may cross the middle
            std::max(l.prefix, l.sum + r.prefix),
            std::max(r.suffix, r.sum + l.suffix)};
}

int main()
{
    std::cout << "3^200 mod 1e9+7 = " << power_mod(3, 200, 1000000007ULL) << '\n';
    std::cout << "2^62 mod 1e18+9 = " << power_mod(2, 62, 1000000000000000009ULL) << '\n';

    std::vector<int> a{8, 4, 2, 1, 7, 3}, tmp(6);
    long inv = merge_count(a, tmp, 0, 6);
    std::cout << "inversions = " << inv << ", sorted:";
    for (int x : a) std::cout << ' ' << x;
    std::cout << '\n';

    std::vector<int> b{3, -4, 5, 2, -1, 6, -8, 4};
    std::cout << "max subarray (divide and conquer) = " << max_subarray(b, 0, static_cast<int>(b.size())).best << '\n';
}
