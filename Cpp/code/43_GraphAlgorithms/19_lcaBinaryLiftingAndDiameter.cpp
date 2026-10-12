// Tree algorithms: LCA with binary lifting, distance between nodes, kth
// ancestor and the tree diameter by two sweeps.

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main()
{
    const int n = 10, LOG = 4;
    std::vector<std::vector<int>> adj(n);
    auto edge = [&](int a, int b) { adj[a].push_back(b); adj[b].push_back(a); };
    edge(0, 1); edge(0, 2); edge(1, 3); edge(1, 4); edge(2, 5); edge(4, 6); edge(4, 7); edge(5, 8); edge(8, 9);

    std::vector<std::vector<int>> up(LOG, std::vector<int>(n, 0));          // up[k][v] = the 2^k-th ancestor of v (the root points to itself)
    std::vector<int> depth(n, 0);
    std::function<void(int, int)> dfs = [&](int u, int p) {
        up[0][u] = p;
        for (int k = 1; k < LOG; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
        for (int v : adj[u]) if (v != p) { depth[v] = depth[u] + 1; dfs(v, u); }
    };
    dfs(0, 0);

    auto kth_ancestor = [&](int v, int k) { for (int b = 0; b < LOG; ++b) if (k >> b & 1) v = up[b][v]; return v; };
    auto lca = [&](int a, int b) {
        if (depth[a] < depth[b]) std::swap(a, b);
        a = kth_ancestor(a, depth[a] - depth[b]);                           // lift the deeper node to the same depth
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; --k) if (up[k][a] != up[k][b]) { a = up[k][a]; b = up[k][b]; }   // jump while the ancestors differ
        return up[0][a];
    };

    for (auto [a, b] : std::vector<std::pair<int, int>>{{3, 6}, {6, 7}, {3, 9}, {7, 7}, {5, 9}})
    {
        int l = lca(a, b);
        std::cout << "lca(" << a << ',' << b << ") = " << l << ", distance " << depth[a] + depth[b] - 2 * depth[l] << '\n';
    }
    std::cout << "3rd ancestor of 9: " << kth_ancestor(9, 3) << '\n';

    auto farthest = [&](int s) {
        std::pair<int, int> best{0, s};
        std::function<void(int, int, int)> go = [&](int u, int p, int d) { best = std::max(best, {d, u}); for (int v : adj[u]) if (v != p) go(v, u, d + 1); };
        go(s, -1, 0);
        return best;
    };
    auto [d1, a] = farthest(0);                                             // the farthest node from anywhere is an end of a diameter
    auto [diameter, b] = farthest(a);
    std::cout << "tree diameter = " << diameter << " edges (from " << a << " to " << b << ")\n";
    std::cout << "binary lifting: O(n log n) build, O(log n) per query; alternatives: Euler tour + sparse table (O(1) query), offline Tarjan\n";
}
