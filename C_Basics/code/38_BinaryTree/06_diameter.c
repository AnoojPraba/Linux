#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode
{
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *createNode(int value)
{
    TreeNode *node = malloc(sizeof(TreeNode));

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode *insert(TreeNode *root, int value)
{
    if (root == NULL)
    {
        return createNode(value);
    }
    if (value < root->value)
    {
        root->left = insert(root->left, value);
    }
    else if (value > root->value)
    {
        root->right = insert(root->right, value);
    }
    return root;
}

/*****************************************************************************
 * Name: height
 *
 * Description:
 *         Computes the height of a (sub)tree (number of edges on the
 *         longest root-to-leaf path, -1 for an empty tree).
 *
 * Inputs:
 *         root : root of the (sub)tree.
 *
 * Returns:
 *         Height of the (sub)tree.
 *****************************************************************************/
int height(TreeNode *root)
{
    int leftHeight;
    int rightHeight;

    if (root == NULL)
    {
        return -1;
    }
    leftHeight = height(root->left);
    rightHeight = height(root->right);
    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

/*****************************************************************************
 * Name: diameterNaive
 *
 * Description:
 *         Naive O(n^2) diameter: at every node, the diameter candidate
 *         through that node is height(left) + height(right) + 2 (edge
 *         count), and the answer is the max of that over all nodes. This
 *         recomputes height() from scratch at every node, so it's O(n)
 *         work per node across O(n) nodes.
 *
 * Inputs:
 *         root : root of the (sub)tree.
 *
 * Returns:
 *         Length (in edges) of the longest path between any two nodes.
 *****************************************************************************/
int diameterNaive(TreeNode *root)
{
    int leftDiameter;
    int rightDiameter;
    int throughRoot;
    int best;

    if (root == NULL)
    {
        return 0;
    }

    throughRoot = height(root->left) + height(root->right) + 2;
    leftDiameter = diameterNaive(root->left);
    rightDiameter = diameterNaive(root->right);

    best = (leftDiameter > rightDiameter) ? leftDiameter : rightDiameter;
    return (throughRoot > best) ? throughRoot : best;
}

/*****************************************************************************
 * Name: diameterHelper
 *
 * Description:
 *         Single-pass O(n) diameter helper: computes and returns the height
 *         of the subtree for the parent's use, while also updating the
 *         running max diameter as a side effect through the out-param. This
 *         avoids the naive approach's repeated height recomputation, since
 *         each node's height is computed exactly once.
 *
 * Inputs:
 *         root      : root of the (sub)tree.
 *         maxDiameter : running best diameter seen so far, updated in place.
 *
 * Returns:
 *         Height of the (sub)tree.
 *****************************************************************************/
int diameterHelper(TreeNode *root, int *maxDiameter)
{
    int leftHeight;
    int rightHeight;
    int throughRoot;

    if (root == NULL)
    {
        return -1;
    }

    leftHeight = diameterHelper(root->left, maxDiameter);
    rightHeight = diameterHelper(root->right, maxDiameter);

    throughRoot = leftHeight + rightHeight + 2;
    if (throughRoot > *maxDiameter)
    {
        *maxDiameter = throughRoot;
    }

    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

/*****************************************************************************
 * Name: diameterOptimized
 *
 * Description:
 *         Wraps diameterHelper to compute the tree diameter in a single
 *         O(n) pass, versus the O(n^2) naive approach above.
 *
 * Inputs:
 *         root : root of the tree.
 *
 * Returns:
 *         Length (in edges) of the longest path between any two nodes.
 *****************************************************************************/
int diameterOptimized(TreeNode *root)
{
    int maxDiameter = 0;

    diameterHelper(root, &maxDiameter);
    return maxDiameter;
}

void freeTree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void)
{
    TreeNode *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10};
    int size = sizeof(values) / sizeof(values[0]);
    int i;

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    printf("naive diameter = %d\n", diameterNaive(root));
    printf("optimized diameter = %d\n", diameterOptimized(root));

    freeTree(root);
    return 0;
}
