#include <stdio.h>
#include <stdlib.h>

// Node layout parallels the plain BST in 38_BinaryTree, plus a height field
// used to compute the balance factor after every insertion.
typedef struct AvlNode
{
    int key;
    int height;
    struct AvlNode *left;
    struct AvlNode *right;
} AvlNode;

/*****************************************************************************
 * Name: nodeHeight
 *
 * Description:
 *         Returns the height of a node, treating NULL as height 0.
 *
 * Inputs:
 *         node : node to measure, may be NULL.
 *
 * Returns:
 *         Height of the node, or 0 if NULL.
 *****************************************************************************/
int nodeHeight(const AvlNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}

/*****************************************************************************
 * Name: maxInt
 *
 * Description:
 *         Returns the larger of two integers.
 *
 * Inputs:
 *         a : first value.
 *         b : second value.
 *
 * Returns:
 *         The larger of a and b.
 *****************************************************************************/
int maxInt(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

/*****************************************************************************
 * Name: createAvlNode
 *
 * Description:
 *         Allocates a new AVL leaf node holding the given key.
 *
 * Inputs:
 *         key : key to store in the new node.
 *
 * Returns:
 *         Pointer to the newly allocated node.
 *****************************************************************************/
AvlNode *createAvlNode(int key)
{
    AvlNode *node = malloc(sizeof(AvlNode));

    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*****************************************************************************
 * Name: balanceFactor
 *
 * Description:
 *         Computes left-height minus right-height for a node; a magnitude
 *         greater than 1 means the AVL invariant is violated.
 *
 * Inputs:
 *         node : node to check, may be NULL.
 *
 * Returns:
 *         The balance factor, or 0 if node is NULL.
 *****************************************************************************/
int balanceFactor(const AvlNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return nodeHeight(node->left) - nodeHeight(node->right);
}

/*****************************************************************************
 * Name: rotateRight
 *
 * Description:
 *         Performs a right rotation around a node to fix a left-heavy
 *         imbalance (the LL case).
 *
 * Inputs:
 *         node : root of the unbalanced subtree.
 *
 * Returns:
 *         New root of the rotated subtree.
 *****************************************************************************/
AvlNode *rotateRight(AvlNode *node)
{
    AvlNode *newRoot = node->left;
    AvlNode *moved = newRoot->right;

    newRoot->right = node;
    node->left = moved;
    node->height = maxInt(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    newRoot->height = maxInt(nodeHeight(newRoot->left), nodeHeight(newRoot->right)) + 1;
    return newRoot;
}

/*****************************************************************************
 * Name: rotateLeft
 *
 * Description:
 *         Performs a left rotation around a node to fix a right-heavy
 *         imbalance (the RR case).
 *
 * Inputs:
 *         node : root of the unbalanced subtree.
 *
 * Returns:
 *         New root of the rotated subtree.
 *****************************************************************************/
AvlNode *rotateLeft(AvlNode *node)
{
    AvlNode *newRoot = node->right;
    AvlNode *moved = newRoot->left;

    newRoot->left = node;
    node->right = moved;
    node->height = maxInt(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    newRoot->height = maxInt(nodeHeight(newRoot->left), nodeHeight(newRoot->right)) + 1;
    return newRoot;
}

/*****************************************************************************
 * Name: avlInsert
 *
 * Description:
 *         Inserts a key into the AVL tree rooted at node, rebalancing via
 *         rotations (LL, RR, LR, RL) as needed to restore the invariant
 *         that every node's balance factor stays within [-1, 1].
 *
 * Inputs:
 *         node : root of the subtree to insert into, may be NULL.
 *         key  : key to insert.
 *
 * Returns:
 *         New root of the (possibly rebalanced) subtree.
 *****************************************************************************/
AvlNode *avlInsert(AvlNode *node, int key)
{
    int balance;

    if (node == NULL)
    {
        return createAvlNode(key);
    }

    if (key < node->key)
    {
        node->left = avlInsert(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = avlInsert(node->right, key);
    }
    else
    {
        return node;
    }

    node->height = maxInt(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    balance = balanceFactor(node);

    if ((balance > 1) && (key < node->left->key))
    {
        return rotateRight(node);
    }
    if ((balance < -1) && (key > node->right->key))
    {
        return rotateLeft(node);
    }
    if ((balance > 1) && (key > node->left->key))
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if ((balance < -1) && (key < node->right->key))
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

/*****************************************************************************
 * Name: inorderPrint
 *
 * Description:
 *         Prints the tree's keys in sorted (in-order) order.
 *
 * Inputs:
 *         node : root of the subtree to print, may be NULL.
 *
 * Returns:
 *         None
 *****************************************************************************/
void inorderPrint(const AvlNode *node)
{
    if (node == NULL)
    {
        return;
    }
    inorderPrint(node->left);
    printf("%d(h=%d) ", node->key, node->height);
    inorderPrint(node->right);
}

int main(void)
{
    AvlNode *root = NULL;
    int keys[] = { 10, 20, 30, 40, 50, 25 };
    int keyCount = sizeof(keys) / sizeof(keys[0]);
    int i;

    for (i = 0; i < keyCount; i++)
    {
        root = avlInsert(root, keys[i]);
    }

    inorderPrint(root);
    printf("\n");

    return 0;
}
