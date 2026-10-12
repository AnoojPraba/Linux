// Sieve of Eratosthenes on a std::bitset / packed bits and the memory
// difference to a byte-per-number sieve.

#include <bitset>
#include <iostream>
#include <vector>

int main()
{
    constexpr std::size_t N = 1'000'000;
    static std::bitset<N + 1> composite;                              // 125 KB instead of 1 MB for vector<char>
    std::size_t count = 0;
    for (std::size_t i = 2; i <= N; ++i)
    {
        if (composite[i]) continue;
        ++count;
        for (std::size_t j = i * i; j <= N; j += i) composite[j] = true;   // start at i*i: smaller multiples were marked already
    }
    std::cout << "primes up to " << N << ": " << count << '\n';
    std::cout << "primes below 50:";
    for (std::size_t i = 2; i < 50; ++i) if (!composite[i]) std::cout << ' ' << i;
    std::cout << "\nmemory: bitset " << sizeof(composite) << " bytes vs vector<char> " << N + 1 << " bytes\n";

    std::vector<std::size_t> twin;                                    // twin primes below 100
    for (std::size_t i = 3; i + 2 < 100; i += 2) if (!composite[i] && !composite[i + 2]) twin.push_back(i);
    std::cout << "twin prime pairs below 100:";
    for (auto p : twin) std::cout << " (" << p << ',' << p + 2 << ')';
    std::cout << '\n';
}
