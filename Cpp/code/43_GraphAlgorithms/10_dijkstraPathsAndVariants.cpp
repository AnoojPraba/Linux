// Dijkstra with a priority_queue: distances, path reconstruction, early exit,
// counting shortest paths and a bottleneck (minimax) path.

#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using Edge = std::pair<int, long>;                                         // (to, weight)
using Graph = std::vector<std::vector<Edge>>;
constexpr long INF = std::numeric_limits<long>::max() / 4;

struct Result { std::vector<long> dist; std::vector<int> parent; std::vector<long> ways; };

Result dijkstra(const Graph &g, int src)
{
    Result r{std::vector<long>(g.size(), INF), std::vector<int>(g.size(), -1), std::vector<long>(g.size(), 0)};
    using Item = std::pair<long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<>> pq;
    r.dist[src] = 0; r.ways[src] = 1;
    pq.push({0, src});
    while (!pq.empty())
    {
        auto [d, u] = pq.top(); pq.pop();
        if (d > r.dist[u]) continue;                                       // stale entry
        for (auto [v, w] : g[u])
        {
            if (d + w < r.dist[v]) { r.dist[v] = d + w; r.parent[v] = u; r.ways[v] = r.ways[u]; pq.push({r.dist[v], v}); }
            else if (d + w == r.dist[v]) r.ways[v] += r.ways[u];           // another shortest path
        }
    }
    return r;
}

long bottleneck(const Graph &g, int s, int t)                              // minimize the largest edge on the path (Dijkstra with max instead of sum)
{
    std::vector<long> best(g.size(), INF);
    using Item = std::pair<long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<>> pq;
    best[s] = 0; pq.push({0, s});
    while (!pq.empty())
    {
        auto [d, u] = pq.top(); pq.pop();
        if (d > best[u]) continue;
        for (auto [v, w] : g[u]) if (std::max(d, w) < best[v]) { best[v] = std::max(d, w); pq.push({best[v], v}); }
    }
    return best[t];
}

int main()
{
    Graph g(7);
    auto edge = [&](int a, int b, long w) { g[a].push_back({b, w}); g[b].push_back({a, w}); };
    edge(0, 1, 4); edge(0, 2, 1); edge(2, 1, 2); edge(1, 3, 1); edge(2, 3, 3); edge(3, 4, 3); edge(5, 6, 1);
    auto r = dijkstra(g, 0);
    for (int v = 0; v < 7; ++v)
    {
        if (r.dist[v] >= INF) { std::cout << "0 -> " << v << ": unreachable\n"; continue; }
        std::vector<int> path;
        for (int x = v; x >= 0; x = r.parent[x]) path.push_back(x);
        std::reverse(path.begin(), path.end());
        std::cout << "0 -> " << v << ": dist " << r.dist[v] << ", " << r.ways[v] << " shortest path(s), path";
        for (int x : path) std::cout << ' ' << x;
        std::cout << '\n';
    }
    std::cout << "bottleneck path 0 -> 4 minimizes the largest edge: " << bottleneck(g, 0, 4) << '\n';
    std::cout << "Dijkstra needs non-negative weights; O((V + E) log V) with a binary heap\n";
}
