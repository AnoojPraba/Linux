// Fenwick tree (prefix sums with point updates) and iterative min segment
// tree.

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

class Fenwick                                            // prefix sums with point updates, O(log n) each
{
    std::vector<long> t_;

public:
    explicit Fenwick(std::size_t n) : t_(n + 1, 0) {}
    void add(std::size_t i, long delta)                  // a[i] += delta (0-based)
    {
        for (++i; i < t_.size(); i += i & -i)            // i & -i is the lowest set bit
            t_[i] += delta;
    }
    long prefix(std::size_t i) const                     // sum of a[0..i)
    {
        long s = 0;
        for (; i > 0; i -= i & -i)
            s += t_[i];
        return s;
    }
    long range(std::size_t l, std::size_t r) const { return prefix(r + 1) - prefix(l); }   // a[l..r]
};

class MinSegmentTree                                     // range minimum with point assignment
{
    std::size_t n_;
    std::vector<int> t_;

public:
    explicit MinSegmentTree(const std::vector<int> &a) : n_(a.size()), t_(2 * a.size(), INT_MAX)
    {
        std::copy(a.begin(), a.end(), t_.begin() + n_);
        for (std::size_t i = n_ - 1; i > 0; --i)
            t_[i] = std::min(t_[2 * i], t_[2 * i + 1]);
    }
    void assign(std::size_t i, int value)
    {
        for (t_[i += n_] = value; i > 1; i /= 2)
            t_[i / 2] = std::min(t_[i], t_[i ^ 1]);
    }
    int range_min(std::size_t l, std::size_t r) const    // min of a[l..r]
    {
        int m = INT_MAX;
        for (l += n_, r += n_ + 1; l < r; l /= 2, r /= 2)
        {
            if (l & 1) m = std::min(m, t_[l++]);
            if (r & 1) m = std::min(m, t_[--r]);
        }
        return m;
    }
};

int main()
{
    std::vector<int> a{5, 2, 8, 1, 9, 3, 7, 4};

    Fenwick f(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) f.add(i, a[i]);
    std::cout << "sum a[2..5] = " << f.range(2, 5) << '\n';
    f.add(3, 10);                                        // a[3] becomes 11
    std::cout << "after a[3] += 10: sum a[2..5] = " << f.range(2, 5) << '\n';

    MinSegmentTree st(a);
    std::cout << "min a[2..6] = " << st.range_min(2, 6) << '\n';
    st.assign(3, 100);                                   // the old minimum is gone
    std::cout << "after a[3] = 100: min a[2..6] = " << st.range_min(2, 6) << '\n';
    std::cout << "min of the whole array = " << st.range_min(0, 7) << '\n';
}
