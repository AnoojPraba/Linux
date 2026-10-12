// Aho-Corasick automaton: search many patterns at once with failure and output
// links in O(n + m + matches).

#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

class AhoCorasick
{
    struct Node
    {
        std::array<int, 26> next;
        int fail = 0;
        int out = -1;                                        // index of a pattern ending here
        int dict = -1;                                       // nearest suffix node that ends a pattern
        Node() { next.fill(-1); }
    };
    std::vector<Node> nodes_{1};
    std::vector<std::string> patterns_;

public:
    void add(const std::string &p)
    {
        int v = 0;
        for (char ch : p)
        {
            int c = ch - 'a';
            if (nodes_[v].next[c] < 0) { nodes_[v].next[c] = static_cast<int>(nodes_.size()); nodes_.emplace_back(); }
            v = nodes_[v].next[c];
        }
        nodes_[v].out = static_cast<int>(patterns_.size());
        patterns_.push_back(p);
    }

    void build()
    {
        std::queue<int> q;
        for (int c = 0; c < 26; ++c)
        {
            if (nodes_[0].next[c] < 0) nodes_[0].next[c] = 0;
            else q.push(nodes_[0].next[c]);
        }
        while (!q.empty())
        {
            int v = q.front(); q.pop();
            for (int c = 0; c < 26; ++c)
            {
                int u = nodes_[v].next[c];
                if (u < 0) { nodes_[v].next[c] = nodes_[nodes_[v].fail].next[c]; continue; }   // complete the automaton
                nodes_[u].fail = nodes_[nodes_[v].fail].next[c];
                int f = nodes_[u].fail;
                nodes_[u].dict = nodes_[f].out >= 0 ? f : nodes_[f].dict;
                q.push(u);
            }
        }
    }

    void search(const std::string &text) const
    {
        int v = 0;
        for (std::size_t i = 0; i < text.size(); ++i)
        {
            v = nodes_[v].next[text[i] - 'a'];
            for (int u = nodes_[v].out >= 0 ? v : nodes_[v].dict; u >= 0; u = nodes_[u].dict)   // report every pattern ending here
                std::cout << "  \"" << patterns_[nodes_[u].out] << "\" ends at index " << i << '\n';
        }
    }
};

int main()
{
    AhoCorasick ac;
    for (const char *p : {"he", "she", "his", "hers"}) ac.add(p);
    ac.build();
    std::cout << "matches in \"ushers\":\n";
    ac.search("ushers");
    std::cout << "one pass over the text regardless of the number of patterns (grep -F, intrusion detection, word filters)\n";
}
