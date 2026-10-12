// Cut vertices and bridges with Tarjan's discovery-time/low-link DFS, handling
// the root and parallel edges.

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

int main()
{
    int n = 9;
    std::vector<std::vector<std::pair<int, int>>> adj(n);                    // (neighbour, edge id): ids make parallel edges safe
    int id = 0;
    auto edge = [&](int a, int b) { adj[a].push_back({b, id}); adj[b].push_back({a, id}); ++id; };
    edge(0, 1); edge(1, 2); edge(2, 0); edge(1, 3); edge(3, 4); edge(4, 5); edge(5, 3); edge(5, 6); edge(7, 8);

    std::vector<int> disc(n, 0), low(n, 0);
    std::vector<bool> art(n, false);
    std::vector<std::pair<int, int>> bridges;
    int timer = 0;
    std::function<void(int, int)> dfs = [&](int u, int parent_edge) {
        disc[u] = low[u] = ++timer;
        int children = 0;
        for (auto [v, eid] : adj[u])
        {
            if (eid == parent_edge) continue;
            if (disc[v]) low[u] = std::min(low[u], disc[v]);               // back edge
            else
            {
                ++children;
                dfs(v, eid);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > disc[u]) bridges.push_back({std::min(u, v), std::max(u, v)});   // nothing below v reaches above u
                if (parent_edge >= 0 && low[v] >= disc[u]) art[u] = true;                    // v's subtree cannot bypass u
            }
        }
        if (parent_edge < 0 && children > 1) art[u] = true;                // the root is a cut vertex only with 2+ DFS children
    };
    for (int i = 0; i < n; ++i) if (!disc[i]) dfs(i, -1);

    std::sort(bridges.begin(), bridges.end());
    std::cout << "bridges:";
    for (auto [a, b] : bridges) std::cout << ' ' << a << '-' << b;
    std::cout << "\narticulation points:";
    for (int i = 0; i < n; ++i) if (art[i]) std::cout << ' ' << i;
    std::cout << "\nremoving a bridge disconnects the graph; removing a cut vertex does too. O(V + E) in one DFS\n";
}
