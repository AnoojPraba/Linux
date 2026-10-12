// 1-D/2-D prefix sums, Kadane, sliding-window maximum with a deque, longest
// substring without repeats.

#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main()
{
    // 1-D prefix sums: any range sum in O(1).
    std::vector<int> a{3, -4, 5, 2, -1, 6, -8, 4};
    std::vector<long> pre(a.size() + 1, 0);
    std::partial_sum(a.begin(), a.end(), pre.begin() + 1);
    std::cout << "sum a[2..5] = " << pre[6] - pre[2] << '\n';

    // 2-D prefix sums: any rectangle sum in O(1).
    std::vector<std::vector<int>> g{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<std::vector<long>> p(4, std::vector<long>(4, 0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            p[i + 1][j + 1] = g[i][j] + p[i][j + 1] + p[i + 1][j] - p[i][j];
    auto rect = [&](int r1, int c1, int r2, int c2) { return p[r2 + 1][c2 + 1] - p[r1][c2 + 1] - p[r2 + 1][c1] + p[r1][c1]; };
    std::cout << "rectangle rows 1..2, cols 1..2 = " << rect(1, 1, 2, 2) << '\n';

    // Kadane: best contiguous sum, with the range.
    int best = a[0], cur = a[0], start = 0, bl = 0, br = 0;
    for (int i = 1; i < static_cast<int>(a.size()); ++i)
    {
        if (cur < 0) { cur = a[i]; start = i; } else cur += a[i];
        if (cur > best) { best = cur; bl = start; br = i; }
    }
    std::cout << "max subarray = " << best << " at [" << bl << ".." << br << "]\n";

    // Sliding-window maximum: a deque of indexes whose values decrease from front to back, O(n).
    std::vector<int> v{1, 3, -1, -3, 5, 3, 6, 7};
    const std::size_t k = 3;
    std::deque<std::size_t> dq;
    std::cout << "window maxima (k=3):";
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        while (!dq.empty() && v[dq.back()] <= v[i]) dq.pop_back();      // smaller values can never be a maximum again
        dq.push_back(i);
        if (dq.front() + k <= i) dq.pop_front();                         // the front fell out of the window
        if (i + 1 >= k) std::cout << ' ' << v[dq.front()];
    }
    std::cout << '\n';

    // Longest substring without a repeated character: a window that grows and shrinks.
    std::string s = "abcabcbbxyzab";
    std::vector<int> last(256, -1);
    int left = 0, longest = 0, where = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right)
    {
        unsigned char c = static_cast<unsigned char>(s[right]);
        if (last[c] >= left) left = last[c] + 1;                         // jump past the previous copy
        last[c] = right;
        if (right - left + 1 > longest) { longest = right - left + 1; where = left; }
    }
    std::cout << "longest unique substring: " << s.substr(where, longest) << " (" << longest << ")\n";
}
