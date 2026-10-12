// Minimum-cost maximum flow with successive shortest paths (SPFA on the
// residual graph): assignment and transport problems.

#include <iostream>
#include <limits>
#include <queue>
#include <vector>

class MinCostFlow
{
    struct Edge { int to, cap, cost; };
    std::vector<Edge> edges_;
    std::vector<std::vector<int>> adj_;

public:
    explicit MinCostFlow(int n) : adj_(n) {}
    void add_edge(int a, int b, int cap, int cost)
    {
        adj_[a].push_back(static_cast<int>(edges_.size())); edges_.push_back({b, cap, cost});
        adj_[b].push_back(static_cast<int>(edges_.size())); edges_.push_back({a, 0, -cost});          // the reverse edge refunds the cost
    }
    std::pair<int, int> run(int s, int t)                                                              // (flow, cost)
    {
        const int INF = std::numeric_limits<int>::max() / 2;
        int flow = 0, cost = 0;
        for (;;)
        {
            std::vector<int> dist(adj_.size(), INF), prev_edge(adj_.size(), -1);
            std::vector<bool> in_q(adj_.size(), false);
            std::queue<int> q;
            dist[s] = 0; q.push(s);
            while (!q.empty())                                                                         // SPFA: negative costs appear on reverse edges
            {
                int u = q.front(); q.pop(); in_q[u] = false;
                for (int id : adj_[u])
                    if (edges_[id].cap > 0 && dist[u] + edges_[id].cost < dist[edges_[id].to])
                    {
                        dist[edges_[id].to] = dist[u] + edges_[id].cost;
                        prev_edge[edges_[id].to] = id;
                        if (!in_q[edges_[id].to]) { in_q[edges_[id].to] = true; q.push(edges_[id].to); }
                    }
            }
            if (dist[t] >= INF) break;
            int push = INF;
            for (int v = t; v != s; v = edges_[prev_edge[v] ^ 1].to) push = std::min(push, edges_[prev_edge[v]].cap);
            for (int v = t; v != s; v = edges_[prev_edge[v] ^ 1].to) { edges_[prev_edge[v]].cap -= push; edges_[prev_edge[v] ^ 1].cap += push; }
            flow += push;
            cost += push * dist[t];
        }
        return {flow, cost};
    }
};

int main()
{
    // assignment problem: 3 workers, 3 jobs, cost matrix; source 0, workers 1..3, jobs 4..6, sink 7
    int cost[3][3] = {{9, 2, 7}, {6, 4, 3}, {5, 8, 1}};
    MinCostFlow g(8);
    for (int w = 0; w < 3; ++w) g.add_edge(0, 1 + w, 1, 0);
    for (int j = 0; j < 3; ++j) g.add_edge(4 + j, 7, 1, 0);
    for (int w = 0; w < 3; ++w) for (int j = 0; j < 3; ++j) g.add_edge(1 + w, 4 + j, 1, cost[w][j]);
    auto [flow, total] = g.run(0, 7);
    std::cout << "assignment: " << flow << " workers assigned, minimum total cost " << total << '\n';

    MinCostFlow t(4);                                                          // transport: 0 -> {1, 2} -> 3, capacities and per-unit costs
    t.add_edge(0, 1, 4, 1); t.add_edge(0, 2, 2, 2); t.add_edge(1, 2, 2, 1); t.add_edge(1, 3, 3, 3); t.add_edge(2, 3, 5, 1);
    auto [f2, c2] = t.run(0, 3);
    std::cout << "transport network: max flow " << f2 << " at minimum cost " << c2 << '\n';
    std::cout << "successive shortest paths: augment along the cheapest path each time; O(F * SPFA); Hungarian algorithm is faster for pure assignment\n";
}
