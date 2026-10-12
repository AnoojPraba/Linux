// Floyd-Warshall all-pairs shortest paths with path reconstruction, negative-
// cycle detection and transitive closure (Warshall).

#include <iostream>
#include <limits>
#include <vector>

constexpr long INF = std::numeric_limits<long>::max() / 4;

int main()
{
    const int n = 5;
    std::vector<std::vector<long>> d(n, std::vector<long>(n, INF));
    std::vector<std::vector<int>> next(n, std::vector<int>(n, -1));      // next[i][j] = the vertex after i on a shortest path to j
    for (int i = 0; i < n; ++i) { d[i][i] = 0; next[i][i] = i; }
    auto edge = [&](int a, int b, long w) { d[a][b] = w; next[a][b] = b; };
    edge(0, 1, 3); edge(0, 2, 8); edge(0, 4, -4); edge(1, 3, 1); edge(1, 4, 7); edge(2, 1, 4); edge(3, 0, 2); edge(3, 2, -5); edge(4, 3, 6);

    for (int k = 0; k < n; ++k)                                           // allow vertex k as an intermediate stop
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (d[i][k] < INF && d[k][j] < INF && d[i][k] + d[k][j] < d[i][j]) { d[i][j] = d[i][k] + d[k][j]; next[i][j] = next[i][k]; }

    bool negative_cycle = false;
    for (int i = 0; i < n; ++i) if (d[i][i] < 0) negative_cycle = true;     // a negative diagonal entry means a negative cycle
    std::cout << "negative cycle: " << std::boolalpha << negative_cycle << "\ndistance matrix:\n";
    for (int i = 0; i < n; ++i) { for (int j = 0; j < n; ++j) std::cout << (d[i][j] >= INF ? "INF" : std::to_string(d[i][j])) << '\t'; std::cout << '\n'; }

    std::cout << "path 2 -> 4:";
    for (int v = 2; v != 4; v = next[v][4]) std::cout << ' ' << v;
    std::cout << " 4 (length " << d[2][4] << ")\n";

    std::vector<std::vector<bool>> reach(4, std::vector<bool>(4, false));   // Warshall: reachability with | and & instead of min and +
    for (int i = 0; i < 4; ++i) reach[i][i] = true;
    reach[0][1] = reach[1][2] = reach[3][2] = true;
    for (int k = 0; k < 4; ++k) for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) if (reach[i][k] && reach[k][j]) reach[i][j] = true;
    std::cout << "transitive closure: 0 reaches 2: " << reach[0][2] << ", 2 reaches 0: " << reach[2][0] << ", 3 reaches 2: " << reach[3][2] << '\n';
    std::cout << "O(V^3) time, O(V^2) space; best for dense graphs or when all pairs are needed (Johnson is better for sparse graphs)\n";
}
