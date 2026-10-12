// BST insert/search/traversals/height; unique_ptr children free the whole tree
// automatically.

#include <algorithm>
#include <iostream>
#include <memory>

struct Node
{
    int key;
    std::unique_ptr<Node> left, right;
    explicit Node(int k) : key(k) {}
};

using Tree = std::unique_ptr<Node>;

void insert(Tree &root, int key)
{
    if (!root)
        root = std::make_unique<Node>(key);
    else if (key < root->key)
        insert(root->left, key);
    else if (key > root->key)
        insert(root->right, key);                      // duplicates are ignored
}

bool search(const Tree &root, int key)
{
    const Node *n = root.get();
    while (n && n->key != key)
        n = key < n->key ? n->left.get() : n->right.get();
    return n != nullptr;
}

int height(const Tree &t) { return t ? 1 + std::max(height(t->left), height(t->right)) : 0; }

void inorder(const Tree &t)   { if (t) { inorder(t->left);  std::cout << t->key << ' '; inorder(t->right); } }
void preorder(const Tree &t)  { if (t) { std::cout << t->key << ' '; preorder(t->left); preorder(t->right); } }
void postorder(const Tree &t) { if (t) { postorder(t->left); postorder(t->right); std::cout << t->key << ' '; } }

int main()
{
    Tree root;
    for (int k : {50, 30, 70, 20, 40, 60, 80, 35})
        insert(root, k);

    std::cout << "in-order  : "; inorder(root);   std::cout << '\n';
    std::cout << "pre-order : "; preorder(root);  std::cout << '\n';
    std::cout << "post-order: "; postorder(root); std::cout << '\n';
    std::cout << "height = " << height(root) << '\n';
    std::cout << std::boolalpha << "search 60: " << search(root, 60) << ", search 65: " << search(root, 65) << '\n';
}
