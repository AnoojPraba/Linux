// Strongly connected components with Kosaraju and Tarjan, and the condensation
// DAG.

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

using Graph = std::vector<std::vector<int>>;

std::vector<int> kosaraju(const Graph &g, int &count)
{
    int n = static_cast<int>(g.size());
    Graph rev(n);
    for (int u = 0; u < n; ++u) for (int v : g[u]) rev[v].push_back(u);
    std::vector<bool> seen(n, false);
    std::vector<int> order;
    std::function<void(int)> dfs1 = [&](int u) { seen[u] = true; for (int v : g[u]) if (!seen[v]) dfs1(v); order.push_back(u); };
    for (int i = 0; i < n; ++i) if (!seen[i]) dfs1(i);
    std::vector<int> comp(n, -1);
    count = 0;
    std::function<void(int)> dfs2 = [&](int u) { comp[u] = count; for (int v : rev[u]) if (comp[v] < 0) dfs2(v); };
    for (int i = n - 1; i >= 0; --i) if (comp[order[i]] < 0) { dfs2(order[i]); ++count; }   // decreasing finish time, on the transpose
    return comp;
}

std::vector<int> tarjan(const Graph &g, int &count)
{
    int n = static_cast<int>(g.size()), timer = 0;
    std::vector<int> idx(n, 0), low(n, 0), comp(n, -1), stack;
    std::vector<bool> on(n, false);
    count = 0;
    std::function<void(int)> dfs = [&](int u) {
        idx[u] = low[u] = ++timer;
        stack.push_back(u); on[u] = true;
        for (int v : g[u])
        {
            if (!idx[v]) { dfs(v); low[u] = std::min(low[u], low[v]); }
            else if (on[v]) low[u] = std::min(low[u], idx[v]);             // an edge back into the current component
        }
        if (low[u] == idx[u])                                              // u is the root of a component: pop it
            for (int v = -1; v != u;) { v = stack.back(); stack.pop_back(); on[v] = false; comp[v] = count; }
        if (low[u] == idx[u]) ++count;
    };
    for (int i = 0; i < n; ++i) if (!idx[i]) dfs(i);
    return comp;
}

int main()
{
    Graph g(8);
    for (auto [a, b] : std::vector<std::pair<int, int>>{{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 5}, {5, 3}, {6, 5}, {6, 7}, {7, 6}}) g[a].push_back(b);
    for (auto fn : {kosaraju, tarjan})
    {
        int count;
        auto comp = fn(g, count);
        std::cout << (fn == kosaraju ? "Kosaraju" : "Tarjan") << " found " << count << " components:";
        for (int c = 0; c < count; ++c)
        {
            std::cout << " {";
            bool first = true;
            for (int v = 0; v < 8; ++v) if (comp[v] == c) { std::cout << (first ? "" : ",") << v; first = false; }
            std::cout << '}';
        }
        std::cout << '\n';
    }
    int count;
    auto comp = tarjan(g, count);
    std::set<std::pair<int, int>> dag;                                        // condensation: components as nodes form a DAG
    for (int u = 0; u < 8; ++u) for (int v : g[u]) if (comp[u] != comp[v]) dag.insert({comp[u], comp[v]});
    std::cout << "condensation edges (component ids):";
    for (auto [a, b] : dag) std::cout << ' ' << a << "->" << b;
    std::cout << "\nTarjan numbers components in reverse topological order of the condensation\n";
}
