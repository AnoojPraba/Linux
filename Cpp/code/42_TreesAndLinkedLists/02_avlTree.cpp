// AVL insertion with the four rotation cases; 1000 sorted keys give height 10.

#include <algorithm>
#include <iostream>
#include <memory>

struct Node
{
    int key, height = 1;
    std::unique_ptr<Node> left, right;
    explicit Node(int k) : key(k) {}
};
using Tree = std::unique_ptr<Node>;

int height(const Tree &t) { return t ? t->height : 0; }
int balance(const Tree &t) { return height(t->left) - height(t->right); }
void update(Tree &t) { t->height = 1 + std::max(height(t->left), height(t->right)); }

// Right rotation: the left child x becomes the root of the subtree and y becomes x's right child.
Tree rotate_right(Tree y)
{
    Tree x = std::move(y->left);
    y->left = std::move(x->right);      // x's old right subtree (keys between x and y) moves under y
    update(y);
    x->right = std::move(y);
    update(x);
    return x;
}

Tree rotate_left(Tree x)
{
    Tree y = std::move(x->right);
    x->right = std::move(y->left);
    update(x);
    y->left = std::move(x);
    update(y);
    return y;
}

Tree insert(Tree t, int key)
{
    if (!t) return std::make_unique<Node>(key);
    if (key < t->key) t->left = insert(std::move(t->left), key);
    else if (key > t->key) t->right = insert(std::move(t->right), key);
    else return t;                                         // no duplicates

    update(t);
    int b = balance(t);
    if (b > 1)                                             // left-heavy
    {
        if (balance(t->left) < 0)                          // left-right case: rotate the child first
            t->left = rotate_left(std::move(t->left));
        return rotate_right(std::move(t));
    }
    if (b < -1)                                            // right-heavy
    {
        if (balance(t->right) > 0)                         // right-left case
            t->right = rotate_right(std::move(t->right));
        return rotate_left(std::move(t));
    }
    return t;
}

bool is_avl(const Tree &t)
{
    if (!t) return true;
    return std::abs(balance(t)) <= 1 && is_avl(t->left) && is_avl(t->right);
}

void inorder(const Tree &t, int &count, int &prev, bool &ordered)
{
    if (!t) return;
    inorder(t->left, count, prev, ordered);
    if (count++ > 0 && t->key <= prev) ordered = false;
    prev = t->key;
    inorder(t->right, count, prev, ordered);
}

int main()
{
    Tree avl;
    for (int k = 1; k <= 1000; ++k)                        // sorted input: a plain BST would become a chain of height 1000
        avl = insert(std::move(avl), k);

    int count = 0, prev = 0;
    bool ordered = true;
    inorder(avl, count, prev, ordered);
    std::cout << "nodes=" << count << " height=" << height(avl) << " balanced=" << std::boolalpha << is_avl(avl)
              << " in-order sorted=" << ordered << '\n';
    std::cout << "minimum possible height for 1000 nodes: 10\n";
    std::cout << "root key = " << avl->key << '\n';
}
