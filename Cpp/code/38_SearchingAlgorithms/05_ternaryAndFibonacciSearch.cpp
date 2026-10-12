// Ternary search for the extremum of a unimodal function (real and integer)
// and Fibonacci search on a sorted array.

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

double ternary_max(const std::function<double(double)> &f, double lo, double hi)
{
    for (int i = 0; i < 100; ++i)                           // each step discards a third of the interval
    {
        double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) lo = m1; else hi = m2;
    }
    return (lo + hi) / 2;
}

int fibonacci_search(const std::vector<int> &a, int key)
{
    std::size_t fib2 = 0, fib1 = 1, fib = 1;
    while (fib < a.size()) { fib2 = fib1; fib1 = fib; fib = fib1 + fib2; }
    long offset = -1;
    while (fib > 1)
    {
        std::size_t i = std::min<long>(offset + static_cast<long>(fib2), static_cast<long>(a.size()) - 1);
        if (a[i] < key)      { fib = fib1; fib1 = fib2; fib2 = fib - fib1; offset = static_cast<long>(i); }
        else if (a[i] > key) { fib = fib2; fib1 -= fib2; fib2 = fib - fib1; }
        else return static_cast<int>(i);
    }
    return (fib1 && offset + 1 < static_cast<long>(a.size()) && a[offset + 1] == key) ? static_cast<int>(offset + 1) : -1;
}

int main()
{
    auto parabola = [](double x) { return -(x - 2.5) * (x - 2.5) + 7; };
    std::cout << "maximum of -(x-2.5)^2 + 7 on [0, 10] at x = " << ternary_max(parabola, 0, 10) << '\n';

    std::vector<int> mountain{1, 4, 9, 14, 11, 6, 2};       // integer ternary/binary search for the peak of a unimodal array
    int lo = 0, hi = static_cast<int>(mountain.size()) - 1;
    while (lo < hi) { int mid = lo + (hi - lo) / 2; if (mountain[mid] < mountain[mid + 1]) lo = mid + 1; else hi = mid; }
    std::cout << "peak of the unimodal array: index " << lo << " (value " << mountain[lo] << ")\n";

    std::vector<int> a{10, 22, 35, 40, 45, 50, 80, 82, 85, 90, 100, 235};
    for (int key : {85, 10, 235, 36}) std::cout << "fibonacci_search(" << key << ") = " << fibonacci_search(a, key) << '\n';
    std::cout << "ternary search does NOT beat binary search on sorted arrays (more comparisons per level); it is for unimodal functions\n";
}
