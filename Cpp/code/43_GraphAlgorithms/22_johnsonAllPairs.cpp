// Johnson's algorithm: all-pairs shortest paths on a sparse graph with
// negative edges via Bellman-Ford potentials and Dijkstra from every vertex.

#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

constexpr long INF = std::numeric_limits<long>::max() / 4;
struct Edge { int u, v; long w; };

int main()
{
    const int n = 4;
    std::vector<Edge> edges{{0, 1, -2}, {1, 2, 3}, {2, 0, 4}, {0, 3, 2}, {3, 2, -1}};

    std::vector<long> h(n + 1, INF);                                      // 1. potentials: Bellman-Ford from a virtual source n with 0-weight edges
    h[n] = 0;
    for (int v = 0; v < n; ++v) h[v] = 0;
    for (int pass = 0; pass < n; ++pass) for (auto &e : edges) if (h[e.u] + e.w < h[e.v]) h[e.v] = h[e.u] + e.w;
    for (auto &e : edges) if (h[e.u] + e.w < h[e.v]) { std::cout << "negative cycle\n"; return 1; }
    std::cout << "potentials h:";
    for (int v = 0; v < n; ++v) std::cout << ' ' << h[v];

    std::vector<std::vector<std::pair<int, long>>> adj(n);                // 2. reweight: w' = w + h[u] - h[v] >= 0 on every edge
    for (auto &e : edges) adj[e.u].push_back({e.v, e.w + h[e.u] - h[e.v]});

    std::cout << "\nall-pairs distances:\n";
    for (int s = 0; s < n; ++s)                                           // 3. Dijkstra from every vertex on the reweighted graph
    {
        std::vector<long> d(n, INF);
        using Item = std::pair<long, int>;
        std::priority_queue<Item, std::vector<Item>, std::greater<>> pq;
        d[s] = 0; pq.push({0, s});
        while (!pq.empty())
        {
            auto [dist, u] = pq.top(); pq.pop();
            if (dist > d[u]) continue;
            for (auto [v, w] : adj[u]) if (dist + w < d[v]) { d[v] = dist + w; pq.push({d[v], v}); }
        }
        std::cout << "  " << s << ':';
        for (int t = 0; t < n; ++t) std::cout << ' ' << (d[t] >= INF ? "INF" : std::to_string(d[t] - h[s] + h[t]));   // undo the reweighting
        std::cout << '\n';
    }
    std::cout << "O(V E log V): better than Floyd-Warshall O(V^3) when the graph is sparse\n";
}
