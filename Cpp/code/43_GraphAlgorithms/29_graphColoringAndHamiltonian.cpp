// NP-hard graph problems by backtracking: m-colouring, Hamiltonian cycle and
// maximum independent set, with a greedy colouring heuristic.

#include <algorithm>
#include <iostream>
#include <vector>

struct Graph
{
    int n;
    std::vector<std::vector<bool>> g;
    explicit Graph(int n_) : n(n_), g(n_, std::vector<bool>(n_, false)) {}
    void edge(int a, int b) { g[a][b] = g[b][a] = true; }
};

bool colour(const Graph &gr, int v, int m, std::vector<int> &col)
{
    if (v == gr.n) return true;
    for (int c = 1; c <= m; ++c)
    {
        bool ok = true;
        for (int u = 0; u < v; ++u) if (gr.g[v][u] && col[u] == c) { ok = false; break; }
        if (ok) { col[v] = c; if (colour(gr, v + 1, m, col)) return true; col[v] = 0; }       // undo and try the next colour
    }
    return false;
}

int greedy_colouring(const Graph &gr)
{
    std::vector<int> col(gr.n, 0);
    int used = 0;
    for (int v = 0; v < gr.n; ++v)
    {
        std::vector<bool> taken(gr.n + 2, false);
        for (int u = 0; u < v; ++u) if (gr.g[v][u]) taken[col[u]] = true;
        int c = 1;
        while (taken[c]) ++c;
        col[v] = c;
        used = std::max(used, c);
    }
    return used;                                                                              // at most max-degree + 1 colours
}

bool hamilton(const Graph &gr, std::vector<int> &path, std::vector<bool> &used)
{
    if (path.size() == static_cast<std::size_t>(gr.n)) return gr.g[path.back()][path.front()];     // must close the cycle
    for (int v = 1; v < gr.n; ++v)
        if (!used[v] && gr.g[path.back()][v]) { used[v] = true; path.push_back(v); if (hamilton(gr, path, used)) return true; path.pop_back(); used[v] = false; }
    return false;
}

int max_independent_set(const Graph &gr)                                                      // by subsets: exponential, fine for n <= ~20
{
    int best = 0;
    for (unsigned mask = 0; mask < (1u << gr.n); ++mask)
    {
        bool ok = true;
        for (int a = 0; a < gr.n && ok; ++a)
            if (mask >> a & 1) for (int b = a + 1; b < gr.n; ++b) if ((mask >> b & 1) && gr.g[a][b]) { ok = false; break; }
        if (ok) best = std::max(best, __builtin_popcount(mask));
    }
    return best;
}

int main()
{
    Graph g(6);
    for (auto [a, b] : std::vector<std::pair<int, int>>{{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 5}}) g.edge(a, b);
    int chi = 0;
    std::vector<int> col(6, 0);
    for (int m = 1; m <= 6; ++m) { std::fill(col.begin(), col.end(), 0); if (colour(g, 0, m, col)) { chi = m; break; } }
    std::cout << "chromatic number " << chi << ", colouring:";
    for (int c : col) std::cout << ' ' << c;
    std::cout << "\ngreedy colouring uses " << greedy_colouring(g) << " colours\n";
    std::vector<int> path{0};
    std::vector<bool> used(6, false);
    used[0] = true;
    std::cout << "Hamiltonian cycle: ";
    if (hamilton(g, path, used)) { for (int v : path) std::cout << v << ' '; std::cout << path[0] << '\n'; } else std::cout << "none\n";
    std::cout << "maximum independent set size: " << max_independent_set(g) << '\n';
}
