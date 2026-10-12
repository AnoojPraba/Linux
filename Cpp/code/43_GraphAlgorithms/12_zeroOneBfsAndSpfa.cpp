// 0-1 BFS with a deque (weights 0 or 1) and SPFA, the queue-based Bellman-Ford
// with negative-cycle detection.

#include <deque>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using Edge = std::pair<int, int>;
using Graph = std::vector<std::vector<Edge>>;
constexpr int INF = std::numeric_limits<int>::max() / 2;

std::vector<int> zero_one_bfs(const Graph &g, int src)
{
    std::vector<int> dist(g.size(), INF);
    std::deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);
    while (!dq.empty())
    {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : g[u])
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v); else dq.push_back(v);         // weight-0 edges go to the front: the deque stays ordered by distance
            }
    }
    return dist;
}

bool spfa(const Graph &g, int src, std::vector<int> &dist)             // returns false if a negative cycle is reachable
{
    int n = static_cast<int>(g.size());
    dist.assign(n, INF);
    std::vector<int> relaxed(n, 0);
    std::vector<bool> in_queue(n, false);
    std::queue<int> q;
    dist[src] = 0; q.push(src); in_queue[src] = true;
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        in_queue[u] = false;
        for (auto [v, w] : g[u])
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                if (++relaxed[v] > n) return false;                         // relaxed more than V times: a negative cycle
                if (!in_queue[v]) { q.push(v); in_queue[v] = true; }
            }
    }
    return true;
}

int main()
{
    Graph a(5);
    a[0] = {{1, 0}, {2, 1}}; a[1] = {{2, 1}, {4, 1}}; a[2] = {{3, 0}}; a[3] = {{4, 1}};
    std::cout << "0-1 BFS distances from 0:";
    for (int d : zero_one_bfs(a, 0)) std::cout << ' ' << d;

    Graph b(4);
    b[0] = {{1, 4}, {2, 5}}; b[1] = {{2, -3}, {3, 6}}; b[2] = {{3, 4}};
    std::vector<int> d;
    bool ok = spfa(b, 0, d);
    std::cout << "\nSPFA with a negative edge: ok=" << std::boolalpha << ok << ", distances:";
    for (int x : d) std::cout << ' ' << x;
    b[3].push_back({1, -2});                                                // 1 -> 2 -> 3 -> 1 sums to -3 + 4 - 2 = -1
    std::cout << "\nafter adding 3 -> 1 (-2): no negative cycle: " << spfa(b, 0, d) << '\n';
}
