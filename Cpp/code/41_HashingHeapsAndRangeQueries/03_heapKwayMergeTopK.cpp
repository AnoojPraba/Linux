// Hand-written binary heap, k-way merge with priority_queue, top-k of a
// stream, make_heap/pop_heap.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class MinHeap                                            // a binary heap stored in an array
{
    std::vector<int> a_;

public:
    void push(int x)
    {
        a_.push_back(x);
        std::size_t i = a_.size() - 1;
        while (i > 0 && a_[(i - 1) / 2] > a_[i])         // sift up
        {
            std::swap(a_[(i - 1) / 2], a_[i]);
            i = (i - 1) / 2;
        }
    }
    int pop()
    {
        int top = a_.front();
        a_.front() = a_.back();
        a_.pop_back();
        std::size_t i = 0;
        for (;;)                                         // sift down
        {
            std::size_t l = 2 * i + 1, r = l + 1, m = i;
            if (l < a_.size() && a_[l] < a_[m]) m = l;
            if (r < a_.size() && a_[r] < a_[m]) m = r;
            if (m == i) break;
            std::swap(a_[i], a_[m]);
            i = m;
        }
        return top;
    }
    bool empty() const { return a_.empty(); }
};

std::vector<int> merge_k(const std::vector<std::vector<int>> &lists)
{
    struct Entry { int value; std::size_t list, index; };
    auto later = [](const Entry &a, const Entry &b) { return a.value > b.value; };
    std::priority_queue<Entry, std::vector<Entry>, decltype(later)> pq(later);
    for (std::size_t i = 0; i < lists.size(); ++i)
        if (!lists[i].empty()) pq.push({lists[i][0], i, 0});

    std::vector<int> out;
    while (!pq.empty())
    {
        Entry e = pq.top();
        pq.pop();
        out.push_back(e.value);
        if (e.index + 1 < lists[e.list].size())                   // replace it with the next element of the same list
            pq.push({lists[e.list][e.index + 1], e.list, e.index + 1});
    }
    return out;
}

int main()
{
    MinHeap h;
    for (int x : {9, 4, 7, 1, 8, 2, 6})
        h.push(x);
    std::cout << "heap order:";
    while (!h.empty()) std::cout << ' ' << h.pop();
    std::cout << '\n';

    auto merged = merge_k({{1, 5, 9, 13}, {2, 6, 10}, {3, 4, 7, 8, 11, 12}});
    std::cout << "merged:";
    for (int x : merged) std::cout << ' ' << x;
    std::cout << '\n';

    std::priority_queue<int, std::vector<int>, std::greater<>> top3;   // min-heap of the 3 largest so far
    for (int x : {15, 3, 42, 8, 23, 4, 16, 99, 1, 61})
    {
        top3.push(x);
        if (top3.size() > 3) top3.pop();
    }
    std::cout << "top 3 of the stream:";
    while (!top3.empty()) { std::cout << ' ' << top3.top(); top3.pop(); }
    std::cout << '\n';

    std::vector<int> v{5, 3, 8, 1, 9, 2};
    std::make_heap(v.begin(), v.end());                                 // O(n) heap construction
    std::pop_heap(v.begin(), v.end());                                  // the maximum moves to the back
    std::cout << "largest via make_heap/pop_heap: " << v.back() << '\n';
}
