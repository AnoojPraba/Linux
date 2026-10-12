// BFS applications: shortest path with reconstruction, grid maze, bipartite
// test, multi-source BFS and level-order grouping.

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using Graph = std::vector<std::vector<int>>;

std::vector<int> shortest_path(const Graph &g, int s, int t)
{
    std::vector<int> parent(g.size(), -2);
    std::queue<int> q;
    parent[s] = -1;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (parent[v] == -2) { parent[v] = u; q.push(v); }
    }
    std::vector<int> path;
    if (parent[t] == -2) return path;
    for (int v = t; v != -1; v = parent[v]) path.push_back(v);
    std::reverse(path.begin(), path.end());
    return path;
}

bool bipartite(const Graph &g)
{
    std::vector<int> side(g.size(), -1);
    for (std::size_t s = 0; s < g.size(); ++s)
    {
        if (side[s] >= 0) continue;
        std::queue<int> q;
        side[s] = 0; q.push(static_cast<int>(s));
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : g[u])
            {
                if (side[v] < 0) { side[v] = !side[u]; q.push(v); }
                else if (side[v] == side[u]) return false;               // an odd cycle
            }
        }
    }
    return true;
}

int maze(const std::vector<std::string> &grid)
{
    int R = static_cast<int>(grid.size()), C = static_cast<int>(grid[0].size());
    std::vector<std::vector<int>> dist(R, std::vector<int>(C, -1));
    std::queue<std::pair<int, int>> q;
    int er = 0, ec = 0;
    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) { if (grid[r][c] == 'S') { dist[r][c] = 0; q.push({r, c}); } if (grid[r][c] == 'E') { er = r; ec = c; } }
    const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (!q.empty())
    {
        auto [r, c] = q.front(); q.pop();
        for (int k = 0; k < 4; ++k)
        {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] != '#' && dist[nr][nc] < 0) { dist[nr][nc] = dist[r][c] + 1; q.push({nr, nc}); }
        }
    }
    return dist[er][ec];
}

int rotting(std::vector<std::vector<int>> g)                            // multi-source BFS: all rotten cells start at distance 0
{
    std::queue<std::pair<int, int>> q;
    int fresh = 0, minutes = 0;
    for (std::size_t r = 0; r < g.size(); ++r) for (std::size_t c = 0; c < g[0].size(); ++c) { if (g[r][c] == 2) q.push({r, c}); if (g[r][c] == 1) ++fresh; }
    const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (!q.empty() && fresh)
    {
        for (std::size_t n = q.size(); n > 0; --n)                      // one whole BFS level = one minute
        {
            auto [r, c] = q.front(); q.pop();
            for (int k = 0; k < 4; ++k)
            {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < static_cast<int>(g.size()) && nc >= 0 && nc < static_cast<int>(g[0].size()) && g[nr][nc] == 1) { g[nr][nc] = 2; --fresh; q.push({nr, nc}); }
            }
        }
        ++minutes;
    }
    return fresh ? -1 : minutes;
}

int main()
{
    Graph g(6);
    auto edge = [&](int a, int b) { g[a].push_back(b); g[b].push_back(a); };
    edge(0, 1); edge(0, 2); edge(1, 3); edge(2, 3); edge(3, 4); edge(4, 5);
    std::cout << "shortest path 0 -> 5:";
    for (int v : shortest_path(g, 0, 5)) std::cout << ' ' << v;
    std::cout << "\nbipartite: " << std::boolalpha << bipartite(g);
    edge(3, 5);
    std::cout << ", after adding edge 3-5 (a triangle): " << bipartite(g) << '\n';
    std::cout << "maze steps: " << maze({"S..#...", ".#.#.#.", ".#...#.", ".####.E", "......."}) << '\n';
    std::cout << "rotting oranges: " << rotting({{2, 1, 1, 0}, {1, 1, 0, 1}, {0, 1, 1, 2}}) << " minutes\n";
}
