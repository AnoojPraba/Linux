// Shortest cycle (girth) of an undirected graph by BFS from every vertex, and
// shortest cycle through a given edge.

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main()
{
    int n = 8;
    std::vector<std::vector<int>> adj(n);
    auto edge = [&](int a, int b) { adj[a].push_back(b); adj[b].push_back(a); };
    edge(0, 1); edge(1, 2); edge(2, 3); edge(3, 4); edge(4, 5); edge(5, 0);      // a 6-cycle
    edge(2, 6); edge(6, 7); edge(7, 3);                                          // plus a 4-cycle 2-3-7-6

    int girth = 1 << 30;
    for (int s = 0; s < n; ++s)
    {
        std::vector<int> dist(n, -1), parent(n, -1);
        std::queue<int> q;
        dist[s] = 0; q.push(s);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : adj[u])
            {
                if (dist[v] < 0) { dist[v] = dist[u] + 1; parent[v] = u; q.push(v); }
                else if (v != parent[u]) girth = std::min(girth, dist[u] + dist[v] + 1);   // a non-tree edge closes a cycle through the BFS tree
            }
        }
    }
    std::cout << "girth (shortest cycle) = " << (girth == (1 << 30) ? -1 : girth) << '\n';

    auto cycle_through_edge = [&](int a, int b) {                               // remove the edge, then the shortest a-b path + 1
        std::vector<int> dist(n, -1);
        std::queue<int> q;
        dist[a] = 0; q.push(a);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : adj[u])
            {
                if ((u == a && v == b) || (u == b && v == a)) continue;
                if (dist[v] < 0) { dist[v] = dist[u] + 1; q.push(v); }
            }
        }
        return dist[b] < 0 ? -1 : dist[b] + 1;
    };
    std::cout << "shortest cycle through edge 2-3: " << cycle_through_edge(2, 3) << ", through edge 0-1: " << cycle_through_edge(0, 1) << ", through edge 6-7: " << cycle_through_edge(6, 7) << '\n';
    std::cout << "O(V (V + E)) for the girth; a tree has no cycle (girth infinite)\n";
}
