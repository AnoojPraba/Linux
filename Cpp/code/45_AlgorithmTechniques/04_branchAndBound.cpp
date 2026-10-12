// Best-first 0/1 knapsack with a fractional bound compared with brute force.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

struct Item { int weight, value; };

struct Node
{
    int level, weight, value;
    double bound;
    bool operator<(const Node &o) const { return bound < o.bound; }   // priority_queue: highest bound first
};

// Optimistic estimate: fill the knapsack with the best value-per-weight items, allowing a fraction of the last one.
double bound_of(const std::vector<Item> &it, int level, int weight, int value, int cap)
{
    double b = value;
    for (std::size_t i = level; i < it.size(); ++i)
    {
        if (weight + it[i].weight <= cap)
        {
            weight += it[i].weight;
            b += it[i].value;
        }
        else
        {
            b += (cap - weight) * static_cast<double>(it[i].value) / it[i].weight;
            break;
        }
    }
    return b;
}

int main()
{
    std::vector<Item> items{{10, 60}, {20, 100}, {30, 120}, {5, 30}, {15, 70}, {25, 90}, {8, 50}, {12, 40}};
    const int cap = 60;
    std::sort(items.begin(), items.end(), [](const Item &a, const Item &b) {
        return static_cast<double>(a.value) / a.weight > static_cast<double>(b.value) / b.weight; });

    int best = 0;
    long expanded = 0;
    std::priority_queue<Node> pq;
    pq.push({0, 0, 0, bound_of(items, 0, 0, 0, cap)});
    while (!pq.empty())
    {
        Node n = pq.top();
        pq.pop();
        if (n.bound <= best || n.level == static_cast<int>(items.size())) continue;   // cannot beat the best: prune
        ++expanded;
        const Item &it = items[n.level];
        if (n.weight + it.weight <= cap)                                       // branch 1: take the item
        {
            Node take{n.level + 1, n.weight + it.weight, n.value + it.value, 0};
            best = std::max(best, take.value);
            take.bound = bound_of(items, take.level, take.weight, take.value, cap);
            if (take.bound > best) pq.push(take);
        }
        Node skip{n.level + 1, n.weight, n.value, 0};                          // branch 2: skip the item
        skip.bound = bound_of(items, skip.level, skip.weight, skip.value, cap);
        if (skip.bound > best) pq.push(skip);
    }

    int brute = 0;
    for (unsigned mask = 0; mask < (1u << items.size()); ++mask)               // brute force: all 2^n subsets
    {
        int w = 0, v = 0;
        for (std::size_t i = 0; i < items.size(); ++i)
            if (mask >> i & 1) { w += items[i].weight; v += items[i].value; }
        if (w <= cap) brute = std::max(brute, v);
    }
    std::cout << "branch and bound best = " << best << ", brute force best = " << brute << '\n';
    std::cout << "nodes expanded: " << expanded << " of " << ((1 << (items.size() + 1)) - 1) << " in the full tree\n";
}
