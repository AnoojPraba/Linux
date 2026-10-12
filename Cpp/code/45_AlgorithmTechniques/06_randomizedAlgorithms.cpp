// Reservoir sampling, randomized quickselect, Miller-Rabin (deterministic
// bases for 64-bit).

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using u64 = unsigned long long;
std::mt19937_64 rng(2024);                              // fixed seed so the output is repeatable

std::vector<int> reservoir(int k, int stream_size)      // k items chosen uniformly from a stream of unknown length
{
    std::vector<int> r;
    for (int i = 0; i < stream_size; ++i)
    {
        if (static_cast<int>(r.size()) < k) r.push_back(i);
        else
        {
            u64 j = rng() % static_cast<u64>(i + 1);    // keep item i with probability k / (i + 1)
            if (j < static_cast<u64>(k)) r[j] = i;
        }
    }
    return r;
}

int quickselect(std::vector<int> a, std::size_t k)      // k-th smallest, 0-based; expected O(n)
{
    std::size_t lo = 0, hi = a.size() - 1;
    while (lo < hi)
    {
        std::size_t p = lo + rng() % (hi - lo + 1);     // random pivot
        std::swap(a[p], a[hi]);
        int pivot = a[hi];
        std::size_t store = lo;
        for (std::size_t i = lo; i < hi; ++i)
            if (a[i] < pivot) std::swap(a[i], a[store++]);
        std::swap(a[store], a[hi]);
        if (k == store) return a[k];
        if (k < store) hi = store - 1; else lo = store + 1;
    }
    return a[lo];
}

u64 mul_mod(u64 a, u64 b, u64 m) { return static_cast<u64>(static_cast<unsigned __int128>(a) * b % m); }

u64 pow_mod(u64 b, u64 e, u64 m)
{
    u64 r = 1;
    for (b %= m; e; e >>= 1, b = mul_mod(b, b, m))
        if (e & 1) r = mul_mod(r, b, m);
    return r;
}

bool is_prime(u64 n)                                    // Miller-Rabin; these 12 bases are deterministic for all n < 3.3e24
{
    if (n < 2) return false;
    for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
    {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }
    for (u64 a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
    {
        u64 x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s && composite; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) composite = false;
        }
        if (composite) return false;                    // a witness proves n is composite
    }
    return true;
}

int main()
{
    auto sample = reservoir(5, 100000);
    std::sort(sample.begin(), sample.end());
    bool valid = std::all_of(sample.begin(), sample.end(), [](int x) { return x >= 0 && x < 100000; });
    std::cout << "reservoir kept 5 distinct valid items: " << std::boolalpha
              << (valid && std::adjacent_find(sample.begin(), sample.end()) == sample.end()) << '\n';

    std::vector<int> data(1001);
    for (int i = 0; i <= 1000; ++i) data[i] = (i * 7919) % 1001;      // a permutation of 0..1000
    std::cout << "median by quickselect = " << quickselect(data, 500) << '\n';

    std::cout << "is_prime(1000000007) = " << is_prime(1000000007ULL) << '\n';
    std::cout << "is_prime(561) (a Carmichael number) = " << is_prime(561) << '\n';
    std::cout << "is_prime(18446744073709551557) = " << is_prime(18446744073709551557ULL) << '\n';
}
