// Bellman-Ford: shortest paths with negative edges, negative-cycle detection
// and printing the cycle itself, plus shortest paths with at most k edges.

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

struct Edge { int u, v, w; };
constexpr long INF = std::numeric_limits<long>::max() / 4;

int main()
{
    int n = 5;
    std::vector<Edge> edges{{0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {3, 4, 2}};     // 1 -> 2 -> 3 -> 1 sums to -3
    std::vector<long> dist(n, 0);
    std::vector<int> parent(n, -1);
    int last = -1;
    for (int pass = 0; pass < n; ++pass)                                   // a relaxation in the V-th pass proves a negative cycle
    {
        last = -1;
        for (auto &e : edges)
            if (dist[e.u] + e.w < dist[e.v]) { dist[e.v] = dist[e.u] + e.w; parent[e.v] = e.u; last = e.v; }
        if (last < 0) break;
    }
    if (last >= 0)
    {
        int x = last;
        for (int i = 0; i < n; ++i) x = parent[x];                         // step back V times to land ON the cycle
        std::vector<int> cycle;
        for (int v = x;; v = parent[v]) { cycle.push_back(v); if (v == x && cycle.size() > 1) break; }
        std::reverse(cycle.begin(), cycle.end());
        std::cout << "negative cycle:";
        for (int v : cycle) std::cout << ' ' << v;
        std::cout << '\n';
    }

    std::vector<Edge> acyclic{{0, 1, 4}, {0, 2, 5}, {1, 2, -3}, {2, 3, 4}, {1, 3, 6}};
    std::vector<long> d(4, INF);
    d[0] = 0;
    for (int pass = 0; pass < 3; ++pass) for (auto &e : acyclic) if (d[e.u] < INF && d[e.u] + e.w < d[e.v]) d[e.v] = d[e.u] + e.w;
    std::cout << "distances with a negative edge:";
    for (long x : d) std::cout << ' ' << x;

    std::vector<long> cur(4, INF);                                         // cheapest path using at most k edges: relax from a copy each round
    cur[0] = 0;
    for (int k = 1; k <= 2; ++k)
    {
        auto next = cur;
        for (auto &e : acyclic) if (cur[e.u] < INF && cur[e.u] + e.w < next[e.v]) next[e.v] = cur[e.u] + e.w;
        cur = next;
        std::cout << "\nat most " << k << " edge(s):";
        for (long x : cur) std::cout << ' ' << (x >= INF ? -1 : x);
    }
    std::cout << "\nBellman-Ford is O(VE); use it when negative edges exist, Dijkstra when they do not\n";
}
