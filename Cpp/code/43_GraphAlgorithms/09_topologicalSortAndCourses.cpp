// Topological sorting three ways: Kahn (BFS), DFS finishing order, and the
// lexicographically smallest order; with cycle detection and course
// scheduling.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using Graph = std::vector<std::vector<int>>;

bool kahn(const Graph &g, std::vector<int> &order, bool smallest_first)
{
    std::vector<int> indeg(g.size(), 0);
    for (auto &adj : g) for (int v : adj) ++indeg[v];
    std::priority_queue<int, std::vector<int>, std::greater<>> ready;     // a min-heap gives the lexicographically smallest order
    std::queue<int> fifo;
    for (std::size_t i = 0; i < g.size(); ++i) if (!indeg[i]) { ready.push(static_cast<int>(i)); fifo.push(static_cast<int>(i)); }
    order.clear();
    while (smallest_first ? !ready.empty() : !fifo.empty())
    {
        int u;
        if (smallest_first) { u = ready.top(); ready.pop(); } else { u = fifo.front(); fifo.pop(); }
        order.push_back(u);
        for (int v : g[u]) if (--indeg[v] == 0) { ready.push(v); fifo.push(v); }
    }
    return order.size() == g.size();                                      // fewer than V means a cycle
}

bool dfs_order(const Graph &g, std::vector<int> &order)
{
    std::vector<int> colour(g.size(), 0);
    bool ok = true;
    std::function<void(int)> dfs = [&](int u) {
        colour[u] = 1;
        for (int v : g[u]) { if (colour[v] == 1) ok = false; else if (colour[v] == 0) dfs(v); }
        colour[u] = 2;
        order.push_back(u);                                               // after all descendants
    };
    for (std::size_t i = 0; i < g.size(); ++i) if (!colour[i]) dfs(static_cast<int>(i));
    std::reverse(order.begin(), order.end());
    return ok;
}

int main()
{
    Graph g(6);
    g[5] = {2, 0}; g[4] = {0, 1}; g[2] = {3}; g[3] = {1};
    std::vector<int> o;
    auto show = [&](const char *name, bool ok) { std::cout << name << ": " << (ok ? "" : "cycle! ") ; for (int v : o) std::cout << v << ' '; std::cout << '\n'; };
    show("Kahn (queue)        ", kahn(g, o, false));
    show("Kahn (smallest first)", kahn(g, o, true));
    o.clear();
    show("DFS finishing order ", dfs_order(g, o));
    g[1].push_back(5);                                                    // 5 -> 2 -> 3 -> 1 -> 5
    std::cout << "after adding 1 -> 5, can finish all courses: " << std::boolalpha << kahn(g, o, false) << '\n';
}
