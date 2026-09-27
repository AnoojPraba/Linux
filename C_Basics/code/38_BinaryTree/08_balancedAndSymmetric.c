#include <stdio.h>
#include <stdlib.h>

#define ABS_DIFF(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

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

/*****************************************************************************
 * Name: checkHeightBalanced
 *
 * Description:
 *         Single-pass check of whether a tree is height-balanced (every
 *         node's left and right subtree heights differ by at most 1,
 *         checked at EVERY node, not just the root). Returns the subtree's
 *         height for the parent's use, and reports imbalance via the
 *         out-param, so an already-found imbalance short-circuits (a height
 *         of -2 is used as a sentinel meaning "already unbalanced,
 *         stop checking further").
 *
 * Inputs:
 *         root      : root of the (sub)tree.
 *         isBalanced : out-param, set to 0 if any subtree is unbalanced.
 *
 * Returns:
 *         Height of the (sub)tree, or an unspecified value once
 *         *isBalanced has already been set to 0.
 *****************************************************************************/
int checkHeightBalanced(TreeNode *root, int *isBalanced)
{
    int leftHeight;
    int rightHeight;

    if (root == NULL)
    {
        return -1;
    }

    leftHeight = checkHeightBalanced(root->left, isBalanced);
    rightHeight = checkHeightBalanced(root->right, isBalanced);

    if (ABS_DIFF(leftHeight, rightHeight) > 1)
    {
        *isBalanced = 0;
    }

    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

/*****************************************************************************
 * Name: isBalanced
 *
 * Description:
 *         Wraps checkHeightBalanced to answer whether the whole tree is
 *         height-balanced.
 *
 * Inputs:
 *         root : root of the tree.
 *
 * Returns:
 *         1 if height-balanced, 0 otherwise.
 *****************************************************************************/
int isBalanced(TreeNode *root)
{
    int balanced = 1;

    checkHeightBalanced(root, &balanced);
    return balanced;
}

/*****************************************************************************
 * Name: isMirror
 *
 * Description:
 *         Checks whether two (sub)trees are mirror images of each other:
 *         their values must match, and the left subtree of one must mirror
 *         the right subtree of the other (and vice versa).
 *
 * Inputs:
 *         a : root of the first (sub)tree.
 *         b : root of the second (sub)tree.
 *
 * Returns:
 *         1 if a and b are mirror images, 0 otherwise.
 *****************************************************************************/
int isMirror(TreeNode *a, TreeNode *b)
{
    if ((a == NULL) && (b == NULL))
    {
        return 1;
    }
    if ((a == NULL) || (b == NULL))
    {
        return 0;
    }
    if (a->value != b->value)
    {
        return 0;
    }
    return isMirror(a->left, b->right) && isMirror(a->right, b->left);
}

/*****************************************************************************
 * Name: isSymmetric
 *
 * Description:
 *         Checks whether a tree is symmetric - a mirror image of itself
 *         around its center - by comparing its left and right subtrees for
 *         mirror-equality.
 *
 * Inputs:
 *         root : root of the tree.
 *
 * Returns:
 *         1 if symmetric, 0 otherwise.
 *****************************************************************************/
int isSymmetric(TreeNode *root)
{
    if (root == NULL)
    {
        return 1;
    }
    return isMirror(root->left, root->right);
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
    TreeNode *balanced = createNode(1);
    TreeNode *unbalanced = createNode(1);
    TreeNode *symmetric = createNode(1);

    balanced->left = createNode(2);
    balanced->right = createNode(3);

    unbalanced->left = createNode(2);
    unbalanced->left->left = createNode(3);
    unbalanced->left->left->left = createNode(4);

    symmetric->left = createNode(2);
    symmetric->right = createNode(2);
    symmetric->left->right = createNode(3);
    symmetric->right->left = createNode(3);

    printf("balanced tree isBalanced = %d\n", isBalanced(balanced));
    printf("unbalanced tree isBalanced = %d\n", isBalanced(unbalanced));
    printf("symmetric tree isSymmetric = %d\n", isSymmetric(symmetric));
    printf("balanced tree isSymmetric = %d\n", isSymmetric(balanced));

    freeTree(balanced);
    freeTree(unbalanced);
    freeTree(symmetric);
    return 0;
}
