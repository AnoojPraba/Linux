// Shortest-path problem variants: at most K stops (layered Bellman-Ford),
// maximum-probability path, network delay time and cheapest path with fuel.

#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

constexpr int INF = std::numeric_limits<int>::max() / 2;

int cheapest_with_stops(int n, const std::vector<std::vector<int>> &flights, int src, int dst, int k)   // at most k stops = k + 1 edges
{
    std::vector<int> cost(n, INF);
    cost[src] = 0;
    for (int i = 0; i <= k; ++i)
    {
        auto next = cost;                                                  // relax from a copy so each round adds exactly one edge
        for (auto &f : flights) if (cost[f[0]] < INF && cost[f[0]] + f[2] < next[f[1]]) next[f[1]] = cost[f[0]] + f[2];
        cost = next;
    }
    return cost[dst] >= INF ? -1 : cost[dst];
}

double max_probability(int n, const std::vector<std::vector<int>> &edges, const std::vector<double> &p, int s, int t)
{
    std::vector<std::vector<std::pair<int, double>>> adj(n);
    for (std::size_t i = 0; i < edges.size(); ++i) { adj[edges[i][0]].push_back({edges[i][1], p[i]}); adj[edges[i][1]].push_back({edges[i][0], p[i]}); }
    std::vector<double> best(n, 0.0);
    std::priority_queue<std::pair<double, int>> pq;                        // a max-heap: Dijkstra for the product of probabilities
    best[s] = 1.0; pq.push({1.0, s});
    while (!pq.empty())
    {
        auto [pr, u] = pq.top(); pq.pop();
        if (pr < best[u]) continue;
        for (auto [v, w] : adj[u]) if (pr * w > best[v]) { best[v] = pr * w; pq.push({best[v], v}); }
    }
    return best[t];
}

int network_delay(int n, const std::vector<std::vector<int>> &times, int k)   // time for a signal from k to reach every node
{
    std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
    for (auto &t : times) adj[t[0]].push_back({t[1], t[2]});
    std::vector<int> d(n + 1, INF);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    d[k] = 0; pq.push({0, k});
    while (!pq.empty())
    {
        auto [dist, u] = pq.top(); pq.pop();
        if (dist > d[u]) continue;
        for (auto [v, w] : adj[u]) if (dist + w < d[v]) { d[v] = dist + w; pq.push({d[v], v}); }
    }
    int worst = *std::max_element(d.begin() + 1, d.end());
    return worst >= INF ? -1 : worst;
}

int main()
{
    std::vector<std::vector<int>> flights{{0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}};
    std::cout << "cheapest 0 -> 3 with at most 1 stop: " << cheapest_with_stops(4, flights, 0, 3, 1) << ", with at most 2 stops: " << cheapest_with_stops(4, flights, 0, 3, 2) << '\n';
    std::cout << "max probability path 0 -> 2: " << max_probability(3, {{0, 1}, {1, 2}, {0, 2}}, {0.5, 0.5, 0.2}, 0, 2) << " (0.25 via 1 beats 0.2 direct)\n";
    std::cout << "network delay from node 2: " << network_delay(4, {{2, 1, 1}, {2, 3, 1}, {3, 4, 1}}, 2) << ", unreachable case: " << network_delay(2, {{1, 2, 1}}, 2) << '\n';
}
