// Eulerian circuits and paths with Hierholzer's algorithm (undirected and
// directed) and the degree conditions for existence.

#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

std::vector<int> undirected_euler(int n, const std::vector<std::pair<int, int>> &edges, int start)
{
    std::vector<std::vector<std::pair<int, int>>> adj(n);                  // (to, edge id)
    for (std::size_t i = 0; i < edges.size(); ++i) { adj[edges[i].first].push_back({edges[i].second, static_cast<int>(i)}); adj[edges[i].second].push_back({edges[i].first, static_cast<int>(i)}); }
    std::vector<bool> used(edges.size(), false);
    std::vector<int> ptr(n, 0), path;
    std::stack<int> st;
    st.push(start);
    while (!st.empty())
    {
        int u = st.top();
        while (ptr[u] < static_cast<int>(adj[u].size()) && used[adj[u][ptr[u]].second]) ++ptr[u];   // skip edges already used
        if (ptr[u] == static_cast<int>(adj[u].size())) { path.push_back(u); st.pop(); }             // stuck: the vertex joins the path
        else { auto [v, id] = adj[u][ptr[u]]; used[id] = true; st.push(v); }
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main()
{
    std::vector<std::pair<int, int>> bowtie{{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2}};
    std::vector<int> deg(5, 0);
    for (auto [a, b] : bowtie) { ++deg[a]; ++deg[b]; }
    int odd = static_cast<int>(std::count_if(deg.begin(), deg.end(), [](int d) { return d % 2; }));
    std::cout << "bow-tie: " << odd << " odd-degree vertices -> " << (odd == 0 ? "Eulerian circuit" : odd == 2 ? "Eulerian path" : "none") << ", route:";
    for (int v : undirected_euler(5, bowtie, 0)) std::cout << ' ' << v;

    std::vector<std::pair<int, int>> tail{{0, 1}, {1, 2}, {2, 3}, {3, 1}, {3, 4}};     // vertices 0 and 4 have odd degree: a path from 0 to 4
    std::cout << "\npath from an odd vertex:";
    for (int v : undirected_euler(5, tail, 0)) std::cout << ' ' << v;

    std::map<int, std::vector<int>> dg{{0, {1}}, {1, {2}}, {2, {0, 3}}, {3, {2}}};     // directed: in-degree == out-degree everywhere
    std::vector<int> route;
    std::stack<int> st;
    st.push(0);
    while (!st.empty())
    {
        int u = st.top();
        if (dg[u].empty()) { route.push_back(u); st.pop(); }
        else { int v = dg[u].back(); dg[u].pop_back(); st.push(v); }
    }
    std::reverse(route.begin(), route.end());
    std::cout << "\ndirected circuit:";
    for (int v : route) std::cout << ' ' << v;
    std::cout << "\nHierholzer is O(E); Eulerian = every edge once (easy), Hamiltonian = every vertex once (NP-complete)\n";
}
