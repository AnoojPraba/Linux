// Graph representations and conversions in C++: edge list, adjacency matrix
// and adjacency list, degrees, density, transpose and weighted edges.

#include <iostream>
#include <utility>
#include <vector>

using EdgeList = std::vector<std::pair<int, int>>;
using Matrix = std::vector<std::vector<int>>;
using AdjList = std::vector<std::vector<int>>;

Matrix to_matrix(const EdgeList &e, int n)
{
    Matrix m(n, std::vector<int>(n, 0));
    for (auto [u, v] : e) m[u][v] = 1;
    return m;
}

AdjList to_list(const Matrix &m)
{
    AdjList g(m.size());
    for (std::size_t u = 0; u < m.size(); ++u)
        for (std::size_t v = 0; v < m.size(); ++v)
            if (m[u][v]) g[u].push_back(static_cast<int>(v));
    return g;
}

AdjList transpose(const AdjList &g)
{
    AdjList t(g.size());
    for (std::size_t u = 0; u < g.size(); ++u) for (int v : g[u]) t[v].push_back(static_cast<int>(u));
    return t;
}

int main()
{
    EdgeList edges{{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}};              // directed
    int n = 5;
    Matrix m = to_matrix(edges, n);
    AdjList g = to_list(m);

    std::cout << "adjacency list:\n";
    for (std::size_t u = 0; u < g.size(); ++u)
    {
        std::cout << "  " << u << ':';
        for (int v : g[u]) std::cout << ' ' << v;
        std::cout << '\n';
    }
    std::vector<int> in(n), out(n);
    for (std::size_t u = 0; u < g.size(); ++u) for (int v : g[u]) { ++out[u]; ++in[v]; }
    std::cout << "(in, out) degrees:";
    for (int i = 0; i < n; ++i) std::cout << ' ' << i << ":(" << in[i] << ',' << out[i] << ')';
    std::cout << "\ndensity: " << edges.size() << " of " << n * (n - 1) << " possible directed edges\n";
    std::cout << "transpose edges:";
    auto t = transpose(g);
    for (std::size_t u = 0; u < t.size(); ++u) for (int v : t[u]) std::cout << ' ' << u << "->" << v;

    std::vector<std::vector<std::pair<int, int>>> wg(3);                  // weighted adjacency list of (to, weight)
    wg[0] = {{1, 7}, {2, 3}};
    wg[1] = {{2, 1}};
    std::cout << "\nweighted: edge 0->2 has weight " << wg[0][1].second << "; matrix memory " << n * n * sizeof(int) << " bytes vs list ~ V + E entries\n";
    std::cout << "matrix: O(1) edge test, O(V^2) space, good for dense graphs; list: O(V + E) space, iterate neighbours fast, good for sparse graphs\n";
}
