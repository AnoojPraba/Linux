// Maximum flow with Edmonds-Karp (BFS augmenting paths on the residual graph),
// the min cut and edge-disjoint paths.

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main()
{
    const int n = 6;
    std::vector<std::vector<int>> cap(n, std::vector<int>(n, 0));
    cap[0][1] = 16; cap[0][2] = 13; cap[1][2] = 10; cap[2][1] = 4; cap[1][3] = 12; cap[3][2] = 9; cap[2][4] = 14; cap[4][3] = 7; cap[3][5] = 20; cap[4][5] = 4;
    auto original = cap;

    auto bfs = [&](int s, int t, std::vector<int> &parent) {
        std::fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        std::queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; ++v)
                if (parent[v] < 0 && cap[u][v] > 0) { parent[v] = u; if (v == t) return true; q.push(v); }   // residual capacity left
        }
        return false;
    };

    int flow = 0;
    std::vector<int> parent(n);
    while (bfs(0, 5, parent))
    {
        int bottleneck = 1 << 30;
        for (int v = 5; v != 0; v = parent[v]) bottleneck = std::min(bottleneck, cap[parent[v]][v]);
        for (int v = 5; v != 0; v = parent[v]) { cap[parent[v]][v] -= bottleneck; cap[v][parent[v]] += bottleneck; }   // forward use + reverse (undo) capacity
        flow += bottleneck;
        std::cout << "augment by " << bottleneck << " (total " << flow << ")\n";
    }
    std::cout << "maximum flow = " << flow << '\n';

    bfs(0, 5, parent);                                                       // reachable from the source in the residual graph = source side of a min cut
    std::cout << "min cut edges:";
    int cut = 0;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (parent[u] >= 0 && parent[v] < 0 && original[u][v] > 0) { std::cout << ' ' << u << "->" << v << '(' << original[u][v] << ')'; cut += original[u][v]; }
    std::cout << "\ncut capacity " << cut << " equals the max flow (max-flow min-cut theorem)\n";
    std::cout << "Edmonds-Karp is O(V E^2); with unit capacities the flow counts edge-disjoint paths\n";
}
