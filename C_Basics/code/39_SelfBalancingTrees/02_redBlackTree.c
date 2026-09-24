#include <stdio.h>
#include <stdlib.h>

// Red-Black tree invariants:
//   1. Every node is colored either RED or BLACK.
//   2. The root is always BLACK.
//   3. Red nodes have only black children (no two reds in a row on any path).
//   4. Every root-to-null-leaf path passes through the same number of black
//      nodes ("black-height"), which is what bounds the tree's height to
//      O(log n) even though the tree is more loosely balanced than an AVL
//      tree (see 01_avlTree.c and NOTES.md for the tradeoff discussion).

#define RED 0
#define BLACK 1

typedef struct RbNode
{
    int key;
    int color;
    struct RbNode *left;
    struct RbNode *right;
    struct RbNode *parent;
} RbNode;

/*****************************************************************************
 * Name: createRbNode
 *
 * Description:
 *         Allocates a new red-colored leaf node holding the given key.
 *         New nodes are always inserted red; recoloring/rotation fixup
 *         restores the invariants afterward.
 *
 * Inputs:
 *         key : key to store in the new node.
 *
 * Returns:
 *         Pointer to the newly allocated node.
 *****************************************************************************/
RbNode *createRbNode(int key)
{
    RbNode *node = malloc(sizeof(RbNode));

    node->key = key;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

/*****************************************************************************
 * Name: rotateLeft
 *
 * Description:
 *         Performs a left rotation around a node, keeping parent links
 *         consistent, and updates root if the rotated node was the root.
 *
 * Inputs:
 *         root : pointer to the tree's root pointer, updated in place.
 *         node : node to rotate around.
 *
 * Returns:
 *         None
 *****************************************************************************/
void rotateLeft(RbNode **root, RbNode *node)
{
    RbNode *pivot = node->right;

    node->right = pivot->left;
    if (pivot->left != NULL)
    {
        pivot->left->parent = node;
    }
    pivot->parent = node->parent;
    if (node->parent == NULL)
    {
        *root = pivot;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = pivot;
    }
    else
    {
        node->parent->right = pivot;
    }
    pivot->left = node;
    node->parent = pivot;
}

/*****************************************************************************
 * Name: rotateRight
 *
 * Description:
 *         Performs a right rotation around a node, keeping parent links
 *         consistent, and updates root if the rotated node was the root.
 *
 * Inputs:
 *         root : pointer to the tree's root pointer, updated in place.
 *         node : node to rotate around.
 *
 * Returns:
 *         None
 *****************************************************************************/
void rotateRight(RbNode **root, RbNode *node)
{
    RbNode *pivot = node->left;

    node->left = pivot->right;
    if (pivot->right != NULL)
    {
        pivot->right->parent = node;
    }
    pivot->parent = node->parent;
    if (node->parent == NULL)
    {
        *root = pivot;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = pivot;
    }
    else
    {
        node->parent->left = pivot;
    }
    pivot->right = node;
    node->parent = pivot;
}

/*****************************************************************************
 * Name: nodeColor
 *
 * Description:
 *         Returns a node's color, treating NULL (a null leaf) as BLACK,
 *         matching the standard red-black convention.
 *
 * Inputs:
 *         node : node to check, may be NULL.
 *
 * Returns:
 *         RED or BLACK.
 *****************************************************************************/
int nodeColor(const RbNode *node)
{
    if (node == NULL)
    {
        return BLACK;
    }
    return node->color;
}

/*****************************************************************************
 * Name: fixupInsert
 *
 * Description:
 *         Restores red-black invariants after inserting a red leaf, via the
 *         standard uncle-color case analysis: a red uncle triggers simple
 *         recoloring (push blackness up), a black uncle triggers a rotation
 *         (with a preceding rotation for the "triangle" left-right/
 *         right-left shapes) followed by recoloring.
 *
 * Inputs:
 *         root : pointer to the tree's root pointer, updated in place.
 *         node : the newly inserted red node to fix up from.
 *
 * Returns:
 *         None
 *****************************************************************************/
void fixupInsert(RbNode **root, RbNode *node)
{
    while (nodeColor(node->parent) == RED)
    {
        RbNode *parent = node->parent;
        RbNode *grandparent = parent->parent;

        if (parent == grandparent->left)
        {
            RbNode *uncle = grandparent->right;

            if (nodeColor(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                if (node == parent->right)
                {
                    node = parent;
                    rotateLeft(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(root, grandparent);
            }
        }
        else
        {
            RbNode *uncle = grandparent->left;

            if (nodeColor(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                if (node == parent->left)
                {
                    node = parent;
                    rotateRight(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(root, grandparent);
            }
        }
    }
    (*root)->color = BLACK;
}

/*****************************************************************************
 * Name: rbInsert
 *
 * Description:
 *         Inserts a key into the red-black tree via plain BST insertion,
 *         then calls fixupInsert to restore the red-black invariants.
 *
 * Inputs:
 *         root : pointer to the tree's root pointer, updated in place.
 *         key  : key to insert.
 *
 * Returns:
 *         None
 *****************************************************************************/
void rbInsert(RbNode **root, int key)
{
    RbNode *parent = NULL;
    RbNode *current = *root;
    RbNode *node;

    while (current != NULL)
    {
        parent = current;
        if (key < current->key)
        {
            current = current->left;
        }
        else if (key > current->key)
        {
            current = current->right;
        }
        else
        {
            return;
        }
    }

    node = createRbNode(key);
    node->parent = parent;
    if (parent == NULL)
    {
        *root = node;
    }
    else if (key < parent->key)
    {
        parent->left = node;
    }
    else
    {
        parent->right = node;
    }

    fixupInsert(root, node);
}

/*****************************************************************************
 * Name: find
 *
 * Description:
 *         Plain BST search for a key, unaffected by node color.
 *
 * Inputs:
 *         root : root of the subtree to search, may be NULL.
 *         key  : key to look for.
 *
 * Returns:
 *         Pointer to the matching node, or NULL if not found.
 *****************************************************************************/
RbNode *find(RbNode *root, int key)
{
    while ((root != NULL) && (root->key != key))
    {
        if (key < root->key)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return root;
}

/*****************************************************************************
 * Name: inorderPrint
 *
 * Description:
 *         Prints the tree's keys in sorted (in-order) order, annotated with
 *         each node's color, to verify both sortedness and coloring.
 *
 * Inputs:
 *         node : root of the subtree to print, may be NULL.
 *
 * Returns:
 *         None
 *****************************************************************************/
void inorderPrint(const RbNode *node)
{
    if (node == NULL)
    {
        return;
    }
    inorderPrint(node->left);
    printf("%d(%s) ", node->key, (node->color == RED) ? "R" : "B");
    inorderPrint(node->right);
}

int main(void)
{
    RbNode *root = NULL;
    RbNode *found;
    // Insertion order chosen to exercise LL, RR, LR, RL rotation shapes and
    // both red-uncle (recolor-only) and black-uncle (rotation) cases.
    int keys[] = { 10, 20, 30, 15, 25, 5, 1, 35, 28, 40 };
    int keyCount = sizeof(keys) / sizeof(keys[0]);
    int i;

    for (i = 0; i < keyCount; i++)
    {
        rbInsert(&root, keys[i]);
    }

    printf("In-order (sorted) with colors: ");
    inorderPrint(root);
    printf("\n");

    found = find(root, 25);
    printf("find(25) = %s\n", (found != NULL) ? "found" : "not found");

    found = find(root, 99);
    printf("find(99) = %s\n", (found != NULL) ? "found" : "not found");

    return 0;
}
