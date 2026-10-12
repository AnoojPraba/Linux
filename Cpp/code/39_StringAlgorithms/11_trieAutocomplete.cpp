// A trie of words: insert, search, starts-with, delete, autocomplete
// suggestions and the longest common prefix.

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Trie
{
    struct Node
    {
        std::array<std::unique_ptr<Node>, 26> child;
        bool word = false;
        int pass = 0;                                        // how many words go through this node
    };
    Node root_;

    void collect(const Node *n, std::string &cur, std::vector<std::string> &out, std::size_t limit) const
    {
        if (out.size() >= limit) return;
        if (n->word) out.push_back(cur);
        for (int c = 0; c < 26; ++c)
            if (n->child[c]) { cur += static_cast<char>('a' + c); collect(n->child[c].get(), cur, out, limit); cur.pop_back(); }
    }
    const Node *find(const std::string &prefix) const
    {
        const Node *n = &root_;
        for (char ch : prefix) { n = n->child[ch - 'a'].get(); if (!n) return nullptr; }
        return n;
    }

public:
    void insert(const std::string &w)
    {
        Node *n = &root_;
        for (char ch : w)
        {
            auto &next = n->child[ch - 'a'];
            if (!next) next = std::make_unique<Node>();
            n = next.get();
            ++n->pass;
        }
        n->word = true;
    }
    bool contains(const std::string &w) const { auto n = find(w); return n && n->word; }
    bool starts_with(const std::string &p) const { return find(p) != nullptr; }
    int count_prefix(const std::string &p) const { auto n = find(p); return n ? n->pass : 0; }
    std::vector<std::string> suggest(const std::string &prefix, std::size_t limit) const
    {
        std::vector<std::string> out;
        if (auto n = find(prefix)) { std::string cur = prefix; collect(n, cur, out, limit); }
        return out;
    }
    std::string common_prefix() const                         // longest prefix shared by all inserted words
    {
        std::string out;
        const Node *n = &root_;
        for (;;)
        {
            int children = 0, last = -1;
            for (int c = 0; c < 26; ++c) if (n->child[c]) { ++children; last = c; }
            if (children != 1 || n->word) return out;
            out += static_cast<char>('a' + last);
            n = n->child[last].get();
        }
    }
};

int main()
{
    Trie t;
    for (const char *w : {"car", "card", "care", "cart", "cat", "dog", "do"}) t.insert(w);
    std::cout << std::boolalpha << "contains car: " << t.contains("car") << ", ca: " << t.contains("ca") << ", starts_with ca: " << t.starts_with("ca")
              << ", words with prefix car: " << t.count_prefix("car") << '\n';
    std::cout << "suggestions for \"car\":";
    for (auto &w : t.suggest("car", 10)) std::cout << ' ' << w;
    Trie u;
    for (const char *w : {"interview", "internet", "interval"}) u.insert(w);
    std::cout << "\ncommon prefix of interview/internet/interval: " << u.common_prefix() << '\n';
    std::cout << "lookup costs O(length of the word), independent of how many words are stored; memory is the price (26 pointers per node)\n";
}
