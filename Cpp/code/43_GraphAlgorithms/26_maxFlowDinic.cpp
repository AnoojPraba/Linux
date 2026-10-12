// Dinic's algorithm: BFS level graph, blocking flow by DFS with current-arc
// pointers, and a bipartite matching solved as a flow.

#include <algorithm>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

class Dinic
{
    struct Edge { int to, cap; };
    std::vector<Edge> edges_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> level_, it_;

    bool bfs(int s, int t)
    {
        std::fill(level_.begin(), level_.end(), -1);
        std::queue<int> q;
        level_[s] = 0; q.push(s);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int id : adj_[u]) if (edges_[id].cap > 0 && level_[edges_[id].to] < 0) { level_[edges_[id].to] = level_[u] + 1; q.push(edges_[id].to); }
        }
        return level_[t] >= 0;
    }
    int dfs(int u, int t, int pushed)
    {
        if (u == t || pushed == 0) return pushed;
        for (int &i = it_[u]; i < static_cast<int>(adj_[u].size()); ++i)      // the current-arc pointer never rescans a dead edge
        {
            int id = adj_[u][i];
            Edge &e = edges_[id];
            if (e.cap > 0 && level_[e.to] == level_[u] + 1)
                if (int got = dfs(e.to, t, std::min(pushed, e.cap)))
                {
                    e.cap -= got;
                    edges_[id ^ 1].cap += got;                                // the paired reverse edge is always index ^ 1
                    return got;
                }
        }
        return 0;
    }

public:
    explicit Dinic(int n) : adj_(n), level_(n), it_(n) {}
    void add_edge(int a, int b, int c)
    {
        adj_[a].push_back(static_cast<int>(edges_.size())); edges_.push_back({b, c});
        adj_[b].push_back(static_cast<int>(edges_.size())); edges_.push_back({a, 0});
    }
    int max_flow(int s, int t, int *phases = nullptr)
    {
        int flow = 0, ph = 0;
        while (bfs(s, t))
        {
            std::fill(it_.begin(), it_.end(), 0);
            while (int f = dfs(s, t, 1 << 30)) flow += f;
            ++ph;
        }
        if (phases) *phases = ph;
        return flow;
    }
};

int main()
{
    Dinic g(6);
    for (auto [a, b, c] : std::vector<std::tuple<int, int, int>>{{0, 1, 16}, {0, 2, 13}, {1, 2, 10}, {2, 1, 4}, {1, 3, 12}, {3, 2, 9}, {2, 4, 14}, {4, 3, 7}, {3, 5, 20}, {4, 5, 4}}) g.add_edge(a, b, c);
    int phases;
    std::cout << "max flow = " << g.max_flow(0, 5, &phases) << " in " << phases << " BFS phases\n";

    Dinic m(2 + 3 + 3);                                                       // bipartite matching as a flow: source 0, workers 1..3, jobs 4..6, sink 7
    for (int w = 1; w <= 3; ++w) m.add_edge(0, w, 1);
    for (int j = 4; j <= 6; ++j) m.add_edge(j, 7, 1);
    for (auto [w, j] : std::vector<std::pair<int, int>>{{1, 4}, {1, 5}, {2, 4}, {3, 5}, {3, 6}}) m.add_edge(w, j, 1);
    std::cout << "maximum matching via flow: " << m.max_flow(0, 7) << '\n';
    std::cout << "Dinic O(V^2 E); O(E sqrt V) on unit-capacity networks (matching, disjoint paths)\n";
}
