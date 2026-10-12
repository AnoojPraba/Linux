// binary, jump, exponential and interpolation search over std::span.

#include <cmath>
#include <iostream>
#include <span>
#include <vector>

struct Result { int index; int probes; };

Result binary(std::span<const int> a, int key, std::size_t lo = 0, std::size_t hi = SIZE_MAX)
{
    int probes = 0;
    if (hi > a.size()) hi = a.size();                      // half-open range [lo, hi)
    while (lo < hi)
    {
        std::size_t mid = lo + (hi - lo) / 2;
        ++probes;
        if (a[mid] == key) return {static_cast<int>(mid), probes};
        if (a[mid] < key) lo = mid + 1; else hi = mid;
    }
    return {-1, probes};
}

Result jump(std::span<const int> a, int key)
{
    const std::size_t step = static_cast<std::size_t>(std::sqrt(static_cast<double>(a.size())));
    std::size_t prev = 0, cur = step;
    int probes = 0;
    while (prev < a.size() && a[std::min(cur, a.size()) - 1] < key)    // jump a block at a time
    {
        ++probes;
        prev = cur;
        cur += step;
    }
    for (std::size_t i = prev; i < std::min(cur, a.size()); ++i)       // scan the one block that can hold key
    {
        ++probes;
        if (a[i] == key) return {static_cast<int>(i), probes};
    }
    return {-1, probes};
}

Result exponential(std::span<const int> a, int key)
{
    if (a.empty()) return {-1, 0};
    int probes = 1;
    if (a[0] == key) return {0, probes};
    std::size_t bound = 1;
    while (bound < a.size() && a[bound] <= key)           // gallop: 1, 2, 4, 8, ...
    {
        ++probes;
        bound *= 2;
    }
    Result r = binary(a, key, bound / 2, std::min(bound + 1, a.size()));
    return {r.index, probes + r.probes};
}

Result interpolation(std::span<const int> a, int key)
{
    int probes = 0;
    long lo = 0, hi = static_cast<long>(a.size()) - 1;
    while (lo <= hi && key >= a[lo] && key <= a[hi])
    {
        ++probes;
        if (a[hi] == a[lo]) return {a[lo] == key ? static_cast<int>(lo) : -1, probes};
        long pos = lo + static_cast<long>(static_cast<double>(key - a[lo]) * (hi - lo) / (a[hi] - a[lo]));
        if (a[pos] == key) return {static_cast<int>(pos), probes};
        if (a[pos] < key) lo = pos + 1; else hi = pos - 1;
    }
    return {-1, probes};
}

int main()
{
    std::vector<int> even, skewed;
    for (int i = 0; i < 1000; ++i) even.push_back(10 + i * 3);          // evenly spread values
    for (int i = 0; i < 1000; ++i) skewed.push_back(i < 990 ? i : 1000000 * (i - 989));   // a few huge values at the end

    auto show = [](const char *name, Result r) { std::cout << name << ": index " << r.index << ", " << r.probes << " probes\n"; };
    std::cout << "evenly spread data, key at index 800:\n";
    show("  binary       ", binary(even, even[800]));
    show("  jump         ", jump(even, even[800]));
    show("  exponential  ", exponential(even, even[800]));
    show("  interpolation", interpolation(even, even[800]));
    std::cout << "key near the front (index 5):\n";
    show("  binary       ", binary(even, even[5]));
    show("  exponential  ", exponential(even, even[5]));
    std::cout << "skewed data, key at index 985:\n";
    show("  binary       ", binary(skewed, skewed[985]));
    show("  interpolation", interpolation(skewed, skewed[985]));
}
