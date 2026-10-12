// A* search on a grid: Manhattan heuristic, path reconstruction and a
// comparison of nodes expanded with Dijkstra and BFS.

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Node { int f, g, r, c; bool operator>(const Node &o) const { return f > o.f; } };

int search(const std::vector<std::string> &grid, bool use_heuristic, int &expanded, std::vector<std::pair<int, int>> *path = nullptr)
{
    int R = static_cast<int>(grid.size()), C = static_cast<int>(grid[0].size()), er = 0, ec = 0;
    for (int r = 0; r < R; ++r) for (int c = 0; c < C; ++c) if (grid[r][c] == 'E') { er = r; ec = c; }
    std::vector<std::vector<int>> best(R, std::vector<int>(C, 1 << 30)), pr(R, std::vector<int>(C, -1)), pc(R, std::vector<int>(C, -1));
    std::priority_queue<Node, std::vector<Node>, std::greater<>> open;
    best[0][0] = 0;
    open.push({0, 0, 0, 0});
    expanded = 0;
    const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (!open.empty())
    {
        Node n = open.top(); open.pop();
        if (n.g > best[n.r][n.c]) continue;
        ++expanded;
        if (n.r == er && n.c == ec)
        {
            if (path) for (int r = er, c = ec; r >= 0;) { path->push_back({r, c}); int nr = pr[r][c], nc = pc[r][c]; r = nr; c = nc; }
            return n.g;
        }
        for (int k = 0; k < 4; ++k)
        {
            int nr = n.r + dr[k], nc = n.c + dc[k];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
            int g = n.g + 1;
            if (g < best[nr][nc])
            {
                best[nr][nc] = g; pr[nr][nc] = n.r; pc[nr][nc] = n.c;
                int h = use_heuristic ? std::abs(nr - er) + std::abs(nc - ec) : 0;   // admissible: never overestimates the real distance
                open.push({g + h, g, nr, nc});
            }
        }
    }
    return -1;
}

int main()
{
    std::vector<std::string> grid{"S.........", ".########.", ".#......#.", ".#.####.#.", ".#.#..#.#.", ".#.#.##.#.", ".#.#....#E", "...#######"};
    int e1, e2;
    std::vector<std::pair<int, int>> path;
    int a = search(grid, true, e1, &path);
    int d = search(grid, false, e2);
    std::cout << "A*      : " << a << " steps, " << e1 << " nodes expanded\nDijkstra: " << d << " steps, " << e2 << " nodes expanded\n";
    std::cout << "path length " << path.size() - 1 << ", starts at (" << path.back().first << ',' << path.back().second << "), ends at (" << path.front().first << ',' << path.front().second << ")\n";
    std::cout << "A* = Dijkstra ordered by g + h; with an admissible heuristic the first time the goal is popped the path is optimal\n";
}
