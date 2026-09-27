#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

// Node layout, height/balance helpers, and rotations mirror 01_avlTree.c;
// this file adds deletion (standard BST delete followed by a rebalancing
// walk back up to the root).
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
 * Name: findMinNode
 *
 * Description:
 *         Finds the left-most (minimum key) node in a subtree, used to
 *         locate the in-order successor when deleting a two-child node.
 *
 * Inputs:
 *         node : root of the subtree to search, must not be NULL.
 *
 * Returns:
 *         Pointer to the minimum-key node.
 *****************************************************************************/
AvlNode *findMinNode(AvlNode *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

/*****************************************************************************
 * Name: avlDelete
 *
 * Description:
 *         Deletes a key from the AVL tree rooted at node via standard BST
 *         deletion (leaf / one-child / two-children via in-order successor),
 *         then walks back up updating heights and applying the correct
 *         rotation (LL, RR, LR, RL) wherever a node becomes unbalanced.
 *
 * Inputs:
 *         node : root of the subtree to delete from, may be NULL.
 *         key  : key to delete.
 *
 * Returns:
 *         New root of the (possibly rebalanced) subtree.
 *****************************************************************************/
AvlNode *avlDelete(AvlNode *node, int key)
{
    int balance;

    if (node == NULL)
    {
        return NULL;
    }

    if (key < node->key)
    {
        node->left = avlDelete(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = avlDelete(node->right, key);
    }
    else
    {
        if ((node->left == NULL) || (node->right == NULL))
        {
            AvlNode *child = (node->left != NULL) ? node->left : node->right;

            if (child == NULL)
            {
                free(node);
                return NULL;
            }
            free(node);
            return child;
        }
        else
        {
            AvlNode *successor = findMinNode(node->right);

            node->key = successor->key;
            node->right = avlDelete(node->right, successor->key);
        }
    }

    node->height = maxInt(nodeHeight(node->left), nodeHeight(node->right)) + 1;
    balance = balanceFactor(node);

    if ((balance > 1) && (balanceFactor(node->left) >= 0))
    {
        printf("  [rotation: LL at node %d]\n", node->key);
        return rotateRight(node);
    }
    if ((balance > 1) && (balanceFactor(node->left) < 0))
    {
        printf("  [rotation: LR at node %d]\n", node->key);
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if ((balance < -1) && (balanceFactor(node->right) <= 0))
    {
        printf("  [rotation: RR at node %d]\n", node->key);
        return rotateLeft(node);
    }
    if ((balance < -1) && (balanceFactor(node->right) > 0))
    {
        printf("  [rotation: RL at node %d]\n", node->key);
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

/*****************************************************************************
 * Name: inorderPrint
 *
 * Description:
 *         Prints the tree's keys in sorted (in-order) order, annotated with
 *         each node's height and balance factor.
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
    printf("%d(h=%d,bf=%d) ", node->key, node->height, balanceFactor(node));
    inorderPrint(node->right);
}

/*****************************************************************************
 * Name: levelOrderPrint
 *
 * Description:
 *         Prints the tree breadth-first, one level's keys per line, to make
 *         the tree's shape (and hence the effect of rotations) visible.
 *
 * Inputs:
 *         root : root of the tree to print, may be NULL.
 *
 * Returns:
 *         None
 *****************************************************************************/
void levelOrderPrint(const AvlNode *root)
{
    const AvlNode *queue[MAX_QUEUE_SIZE];
    int head = 0;
    int tail = 0;
    int levelCount;
    int i;

    if (root == NULL)
    {
        printf("(empty)\n");
        return;
    }

    queue[tail] = root;
    tail = tail + 1;

    while (head < tail)
    {
        levelCount = tail - head;
        for (i = 0; i < levelCount; i++)
        {
            const AvlNode *current = queue[head];

            head = head + 1;
            printf("%d(bf=%d) ", current->key, balanceFactor(current));
            if (current->left != NULL)
            {
                queue[tail] = current->left;
                tail = tail + 1;
            }
            if (current->right != NULL)
            {
                queue[tail] = current->right;
                tail = tail + 1;
            }
        }
        printf("\n");
    }
}

/*****************************************************************************
 * Name: showTree
 *
 * Description:
 *         Prints a labeled in-order and level-order snapshot of the tree,
 *         used before/after each deletion to make correctness (sortedness)
 *         and rebalancing (shape/balance factors) visible.
 *
 * Inputs:
 *         label : short description printed before the traversals.
 *         root  : root of the tree to print.
 *
 * Returns:
 *         None
 *****************************************************************************/
void showTree(const char *label, const AvlNode *root)
{
    printf("%s\n", label);
    printf("  in-order:    ");
    inorderPrint(root);
    printf("\n");
    printf("  level-order:\n  ");
    levelOrderPrint(root);
}

int main(void)
{
    AvlNode *root = NULL;
    // Insertion order builds a tree with several 2-level-deep subtrees, so
    // deleting from opposite sides triggers both a single (LL) and a double
    // (RL) rebalancing rotation on the way back up.
    int keys[] = { 9, 5, 10, 0, 6, 11, -1, 1, 2 };
    int keyCount = sizeof(keys) / sizeof(keys[0]);
    int i;

    for (i = 0; i < keyCount; i++)
    {
        root = avlInsert(root, keys[i]);
    }

    showTree("Initial tree:", root);

    // Deleting 11 removes the only node on the right of the root's right
    // child's subtree; the root's left side (rooted at 1) is heavier,
    // triggering a rebalance as the fix walks up.
    printf("\nDeleting 11...\n");
    root = avlDelete(root, 11);
    showTree("After deleting 11:", root);

    // Deleting 10 further thins the right side, cascading another rotation.
    printf("\nDeleting 10...\n");
    root = avlDelete(root, 10);
    showTree("After deleting 10:", root);

    // Deleting 2 leaves node 5's right subtree (rooted at 9, which itself
    // has a left child) heavier than its (now missing) left child, a
    // right-left shape that requires the RL double rotation to fix.
    printf("\nDeleting 2...\n");
    root = avlDelete(root, 2);
    showTree("After deleting 2:", root);

    return 0;
}
