// BFS shortest path, DFS topological sort with colour-based cycle detection,
// Dijkstra with priority_queue.

#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using Graph = std::vector<std::vector<int>>;
using WGraph = std::vector<std::vector<std::pair<int, int>>>;     // (neighbor, weight)

std::vector<int> bfs_distance(const Graph &g, int src)
{
    std::vector<int> dist(g.size(), -1);
    std::queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : g[u])
            if (dist[v] < 0)
            {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return dist;
}

// Returns false if the directed graph has a cycle; otherwise fills order.
bool topo_sort(const Graph &g, std::vector<int> &order)
{
    enum { White, Grey, Black };
    std::vector<int> colour(g.size(), White);
    bool ok = true;
    std::function<void(int)> dfs = [&](int u) {
        colour[u] = Grey;                                // on the current DFS path
        for (int v : g[u])
        {
            if (colour[v] == Grey) ok = false;           // back edge: a cycle
            else if (colour[v] == White) dfs(v);
        }
        colour[u] = Black;
        order.push_back(u);                              // after all descendants
    };
    for (int u = 0; u < static_cast<int>(g.size()); ++u)
        if (colour[u] == White) dfs(u);
    std::reverse(order.begin(), order.end());            // reverse finishing order
    return ok;
}

std::vector<long> dijkstra(const WGraph &g, int src)
{
    const long INF = std::numeric_limits<long>::max();
    std::vector<long> dist(g.size(), INF);
    using Item = std::pair<long, int>;                   // (distance, vertex)
    std::priority_queue<Item, std::vector<Item>, std::greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;                       // stale entry: a shorter path was already found
        for (auto [v, w] : g[u])
            if (d + w < dist[v])
            {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
    }
    return dist;
}

int main()
{
    Graph u(6);                                          // undirected, unweighted
    auto edge = [&](int a, int b) { u[a].push_back(b); u[b].push_back(a); };
    edge(0, 1); edge(0, 2); edge(1, 3); edge(2, 4); edge(3, 5); edge(4, 5);
    std::cout << "BFS distances from 0:";
    for (int d : bfs_distance(u, 0)) std::cout << ' ' << d;
    std::cout << '\n';

    Graph dag{{1, 2}, {3}, {3}, {4}, {}};                // edges 0->1 0->2 1->3 2->3 3->4
    std::vector<int> order;
    std::cout << "acyclic: " << std::boolalpha << topo_sort(dag, order) << ", order:";
    for (int v : order) std::cout << ' ' << v;
    std::cout << '\n';
    dag[4].push_back(0);                                 // 4 -> 0 closes a cycle
    order.clear();
    std::cout << "with back edge, acyclic: " << topo_sort(dag, order) << '\n';

    WGraph w(5);
    auto wedge = [&](int a, int b, int c) { w[a].push_back({b, c}); w[b].push_back({a, c}); };
    wedge(0, 1, 4); wedge(0, 2, 1); wedge(2, 1, 2); wedge(1, 3, 1); wedge(2, 3, 5); wedge(3, 4, 3);
    std::cout << "Dijkstra from 0:";
    for (long d : dijkstra(w, 0)) std::cout << ' ' << d;
    std::cout << '\n';
}
