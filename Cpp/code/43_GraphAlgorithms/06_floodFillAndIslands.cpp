// Grid graphs: flood fill, number of islands (DFS and union-find), largest
// island and surrounded regions.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Grid
{
    std::vector<std::string> g;
    int R() const { return static_cast<int>(g.size()); }
    int C() const { return static_cast<int>(g[0].size()); }
    int fill(int r, int c, char from, char to)                          // 4-directional flood fill, returns the filled area
    {
        if (r < 0 || r >= R() || c < 0 || c >= C() || g[r][c] != from) return 0;
        g[r][c] = to;
        return 1 + fill(r + 1, c, from, to) + fill(r - 1, c, from, to) + fill(r, c + 1, from, to) + fill(r, c - 1, from, to);
    }
};

struct Dsu
{
    std::vector<int> p;
    explicit Dsu(int n) : p(n) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) { a = find(a); b = find(b); if (a == b) return false; p[a] = b; return true; }
};

int main()
{
    Grid grid{{"11000.", "11000.", "00100.", "0001.1", "00011."}};
    Dsu dsu(grid.R() * grid.C());
    int comps = 0;
    for (int r = 0; r < grid.R(); ++r)
        for (int c = 0; c < grid.C(); ++c)
            if (grid.g[r][c] == '1')
            {
                ++comps;
                if (r + 1 < grid.R() && grid.g[r + 1][c] == '1' && dsu.unite(r * grid.C() + c, (r + 1) * grid.C() + c)) --comps;
                if (c + 1 < grid.C() && grid.g[r][c + 1] == '1' && dsu.unite(r * grid.C() + c, r * grid.C() + c + 1)) --comps;
            }
    std::cout << "islands by union-find: " << comps << '\n';

    int islands = 0, biggest = 0;
    for (int r = 0; r < grid.R(); ++r)
        for (int c = 0; c < grid.C(); ++c)
            if (grid.g[r][c] == '1') { biggest = std::max(biggest, grid.fill(r, c, '1', '#')); ++islands; }
    std::cout << "islands by flood fill: " << islands << ", largest has " << biggest << " cells\n";
    for (auto &row : grid.g) std::cout << "  " << row << '\n';

    Grid board{{"XXXX", "XOOX", "XXOX", "XOXX"}};                      // surrounded regions: O cells not connected to the border become X
    for (int r = 0; r < board.R(); ++r) for (int c = 0; c < board.C(); ++c) if ((r == 0 || c == 0 || r == board.R() - 1 || c == board.C() - 1) && board.g[r][c] == 'O') board.fill(r, c, 'O', '*');
    for (auto &row : board.g) for (char &ch : row) ch = ch == '*' ? 'O' : (ch == 'O' ? 'X' : ch);
    std::cout << "surrounded regions captured:\n";
    for (auto &row : board.g) std::cout << "  " << row << '\n';
}
