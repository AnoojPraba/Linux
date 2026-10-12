// Huffman coding with a priority_queue: build the optimal prefix code, encode
// and decode, and compare with fixed-width coding.

#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

struct Node
{
    int freq;
    char ch;
    std::unique_ptr<Node> left, right;
    Node(int f, char c) : freq(f), ch(c) {}
    Node(int f, std::unique_ptr<Node> l, std::unique_ptr<Node> r) : freq(f), ch(0), left(std::move(l)), right(std::move(r)) {}
};

void assign_codes(const Node *n, std::string prefix, std::map<char, std::string> &codes)
{
    if (!n->left && !n->right) { codes[n->ch] = prefix.empty() ? "0" : prefix; return; }
    assign_codes(n->left.get(), prefix + '0', codes);
    assign_codes(n->right.get(), prefix + '1', codes);
}

int main()
{
    std::string text = "abracadabra alakazam";
    std::map<char, int> freq;
    for (char c : text) ++freq[c];

    auto cmp = [](const std::unique_ptr<Node> &a, const std::unique_ptr<Node> &b) { return a->freq > b->freq; };
    std::priority_queue<std::unique_ptr<Node>, std::vector<std::unique_ptr<Node>>, decltype(cmp)> pq(cmp);
    for (auto [c, f] : freq) pq.push(std::make_unique<Node>(f, c));
    while (pq.size() > 1)                                              // merge the two rarest subtrees
    {
        auto a = std::move(const_cast<std::unique_ptr<Node> &>(pq.top())); pq.pop();
        auto b = std::move(const_cast<std::unique_ptr<Node> &>(pq.top())); pq.pop();
        int f = a->freq + b->freq;
        pq.push(std::make_unique<Node>(f, std::move(a), std::move(b)));
    }
    std::map<char, std::string> codes;
    assign_codes(pq.top().get(), "", codes);

    std::string encoded;
    for (char c : text) encoded += codes[c];
    for (auto &[c, code] : codes) std::cout << '\'' << (c == ' ' ? '_' : c) << "' freq " << freq[c] << " code " << code << '\n';
    std::cout << "encoded: " << encoded.size() << " bits instead of " << text.size() * 8 << '\n';

    std::string decoded;                                               // decode by walking the tree: no code is a prefix of another
    const Node *n = pq.top().get();
    for (char bit : encoded)
    {
        n = bit == '0' ? n->left.get() : n->right.get();
        if (!n->left && !n->right) { decoded += n->ch; n = pq.top().get(); }
    }
    std::cout << "decoded == original: " << std::boolalpha << (decoded == text) << '\n';
}
