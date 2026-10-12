// Minimum spanning trees beyond Kruskal: Prim with a priority_queue and
// Boruvka (cheapest edge of every component per round).

#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

struct Edge { int u, v, w; };

long prim(int n, const std::vector<Edge> &edges)
{
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (auto &e : edges) { adj[e.u].push_back({e.v, e.w}); adj[e.v].push_back({e.u, e.w}); }
    std::vector<bool> in_tree(n, false);
    using Item = std::pair<int, int>;                                      // (weight, vertex)
    std::priority_queue<Item, std::vector<Item>, std::greater<>> pq;
    pq.push({0, 0});
    long total = 0;
    while (!pq.empty())
    {
        auto [w, u] = pq.top(); pq.pop();
        if (in_tree[u]) continue;                                          // lazy deletion
        in_tree[u] = true;
        total += w;
        for (auto [v, wt] : adj[u]) if (!in_tree[v]) pq.push({wt, v});
    }
    return total;
}

long boruvka(int n, const std::vector<Edge> &edges)
{
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);
    std::function<int(int)> find = [&](int x) { return p[x] == x ? x : p[x] = find(p[x]); };
    long total = 0;
    int comps = n;
    while (comps > 1)
    {
        std::vector<int> cheapest(n, -1);
        for (std::size_t i = 0; i < edges.size(); ++i)
        {
            int a = find(edges[i].u), b = find(edges[i].v);
            if (a == b) continue;
            for (int c : {a, b}) if (cheapest[c] < 0 || edges[i].w < edges[cheapest[c]].w) cheapest[c] = static_cast<int>(i);
        }
        bool merged = false;
        for (int c = 0; c < n; ++c)
            if (cheapest[c] >= 0)
            {
                int a = find(edges[cheapest[c]].u), b = find(edges[cheapest[c]].v);
                if (a != b) { p[a] = b; total += edges[cheapest[c]].w; --comps; merged = true; }
            }
        if (!merged) break;                                                // disconnected graph
    }
    return total;
}

int main()
{
    std::vector<Edge> edges{{0, 1, 7}, {0, 3, 5}, {1, 2, 8}, {1, 3, 9}, {1, 4, 7}, {2, 4, 5}, {3, 4, 15}, {3, 5, 6}, {4, 5, 8}, {4, 6, 9}, {5, 6, 11}};
    std::cout << "Prim    : " << prim(7, edges) << '\n';
    std::cout << "Boruvka : " << boruvka(7, edges) << '\n';
    std::cout << "Prim O(E log V) with a heap (grows one tree), Boruvka O(E log V) in log V rounds (parallel friendly), Kruskal sorts edges (see 02_unionFindKruskal)\n";
}
