// DFS applications: connected components, cycle detection (undirected and
// directed), entry/exit times and edge classification, path finding.

#include <functional>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<int>>;

int main()
{
    Graph und(7);
    auto add = [&](int a, int b) { und[a].push_back(b); und[b].push_back(a); };
    add(0, 1); add(1, 2); add(3, 4); add(5, 6);
    std::vector<int> comp(7, -1);
    int count = 0;
    std::function<void(int)> label = [&](int u) { comp[u] = count; for (int v : und[u]) if (comp[v] < 0) label(v); };
    for (int i = 0; i < 7; ++i) if (comp[i] < 0) { label(i); ++count; }
    std::cout << count << " connected components:";
    for (int c : comp) std::cout << ' ' << c;

    auto has_cycle = [&](const Graph &g) {                                // undirected: an edge to a visited vertex that is not the parent
        std::vector<bool> seen(g.size(), false);
        std::function<bool(int, int)> dfs = [&](int u, int p) {
            seen[u] = true;
            for (int v : g[u]) if (!seen[v] ? dfs(v, u) : v != p) return true;
            return false;
        };
        for (std::size_t i = 0; i < g.size(); ++i) if (!seen[i] && dfs(static_cast<int>(i), -1)) return true;
        return false;
    };
    std::cout << "\ncycle in the forest: " << std::boolalpha << has_cycle(und);
    add(2, 0);
    std::cout << ", after adding edge 2-0: " << has_cycle(und) << '\n';

    Graph dir(4);
    dir[0] = {1, 2}; dir[1] = {3}; dir[2] = {3}; dir[3] = {1};
    std::vector<int> colour(4, 0), tin(4, 0), tout(4, 0);                // 0 white, 1 grey (on the path), 2 black
    int timer = 0;
    std::function<void(int)> classify = [&](int u) {
        colour[u] = 1;
        tin[u] = ++timer;
        for (int v : dir[u])
        {
            if (!tin[v]) { std::cout << "  tree edge    " << u << "->" << v << '\n'; classify(v); }
            else if (colour[v] == 1) std::cout << "  back edge    " << u << "->" << v << " (a cycle)\n";
            else if (tin[u] < tin[v]) std::cout << "  forward edge " << u << "->" << v << '\n';
            else std::cout << "  cross edge   " << u << "->" << v << '\n';
        }
        colour[u] = 2;
        tout[u] = ++timer;
    };
    std::cout << "directed edge classification:\n";
    classify(0);

    std::vector<int> path;                                                // any path from 0 to 3 by backtracking DFS
    std::vector<bool> visited(4, false);
    std::function<bool(int)> find = [&](int u) {
        path.push_back(u);
        visited[u] = true;
        if (u == 3) return true;
        for (int v : dir[u]) if (!visited[v] && find(v)) return true;
        path.pop_back();
        return false;
    };
    find(0);
    std::cout << "a path 0 -> 3:";
    for (int v : path) std::cout << ' ' << v;
    std::cout << "\nrecursion depth can overflow the stack on huge graphs: use an explicit std::stack there\n";
}
