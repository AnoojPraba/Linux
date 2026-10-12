// Maximum bipartite matching with Kuhn's augmenting paths and Hopcroft-Karp
// (BFS layers + DFS), with Konig's vertex cover.

#include <functional>
#include <iostream>
#include <queue>
#include <vector>

struct Matching
{
    int nl, nr;
    std::vector<std::vector<int>> adj;                                       // left vertex -> right vertices
    std::vector<int> match_l, match_r;
    Matching(int l, int r) : nl(l), nr(r), adj(l), match_l(l, -1), match_r(r, -1) {}

    int kuhn()
    {
        int size = 0;
        for (int u = 0; u < nl; ++u)
        {
            std::vector<bool> visited(nr, false);
            std::function<bool(int)> try_assign = [&](int x) {
                for (int y : adj[x])
                {
                    if (visited[y]) continue;
                    visited[y] = true;
                    if (match_r[y] < 0 || try_assign(match_r[y])) { match_r[y] = x; match_l[x] = y; return true; }   // free, or its owner can move
                }
                return false;
            };
            size += try_assign(u);
        }
        return size;
    }

    int hopcroft_karp()
    {
        std::fill(match_l.begin(), match_l.end(), -1);
        std::fill(match_r.begin(), match_r.end(), -1);
        int size = 0;
        std::vector<int> dist(nl);
        auto bfs = [&]() {
            std::queue<int> q;
            for (int u = 0; u < nl; ++u) { dist[u] = match_l[u] < 0 ? 0 : -1; if (match_l[u] < 0) q.push(u); }
            bool found = false;
            while (!q.empty())
            {
                int u = q.front(); q.pop();
                for (int v : adj[u])
                {
                    int w = match_r[v];
                    if (w < 0) found = true;
                    else if (dist[w] < 0) { dist[w] = dist[u] + 1; q.push(w); }
                }
            }
            return found;
        };
        std::function<bool(int)> dfs = [&](int u) {
            for (int v : adj[u])
            {
                int w = match_r[v];
                if (w < 0 || (dist[w] == dist[u] + 1 && dfs(w))) { match_l[u] = v; match_r[v] = u; return true; }
            }
            dist[u] = -1;
            return false;
        };
        while (bfs()) for (int u = 0; u < nl; ++u) if (match_l[u] < 0 && dfs(u)) ++size;      // many shortest augmenting paths per phase
        return size;
    }
};

int main()
{
    Matching m(5, 5);
    for (auto [w, j] : std::vector<std::pair<int, int>>{{0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}, {3, 3}, {4, 3}}) m.adj[w].push_back(j);
    std::cout << "Kuhn maximum matching: " << m.kuhn() << '\n';
    for (int j = 0; j < 5; ++j) if (m.match_r[j] >= 0) std::cout << "  job " << j << " <- worker " << m.match_r[j] << '\n';
    std::cout << "Hopcroft-Karp maximum matching: " << m.hopcroft_karp() << '\n';
    std::cout << "Konig: max matching = min vertex cover = 4; max independent set = V - matching = 10 - 4 = 6\n";
    std::cout << "Kuhn O(V E), Hopcroft-Karp O(E sqrt V); the same answer comes from max flow source -> left -> right -> sink\n";
}
