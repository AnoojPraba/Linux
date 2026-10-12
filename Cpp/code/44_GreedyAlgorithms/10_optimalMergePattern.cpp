// Optimal merge pattern and connecting ropes: always merge the two smallest
// with a min-heap (the Huffman rule).

#include <functional>
#include <iostream>
#include <queue>
#include <vector>

long cheapest_merge(const std::vector<int> &sizes, bool verbose)
{
    std::priority_queue<int, std::vector<int>, std::greater<>> pq(sizes.begin(), sizes.end());
    long cost = 0;
    while (pq.size() > 1)
    {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        if (verbose) std::cout << "  merge " << a << " + " << b << " -> " << a + b << '\n';
        cost += a + b;
        pq.push(a + b);
    }
    return cost;
}

int main()
{
    std::vector<int> files{20, 30, 10, 5, 30};
    std::cout << "optimal merge of 20 30 10 5 30:\n";
    long total = cheapest_merge(files, true);                          // evaluated first so the steps print before the summary
    std::cout << "total cost " << total << '\n';
    std::cout << "connect ropes 4 3 2 6: " << cheapest_merge({4, 3, 2, 6}, false) << '\n';
    long naive = 0, acc = files[0];
    for (std::size_t i = 1; i < files.size(); ++i) { acc += files[i]; naive += acc; }
    std::cout << "merging left to right costs " << naive << " (larger)\n";
}
