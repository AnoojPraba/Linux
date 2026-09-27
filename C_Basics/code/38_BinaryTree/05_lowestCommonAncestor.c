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
 * Name: lowestCommonAncestorBST
 *
 * Description:
 *         Finds the lowest common ancestor of two values in a BST by
 *         exploiting BST ordering: walk down from the root, and if both
 *         targets are smaller than the current node, the LCA must be in the
 *         left subtree; if both are larger, it must be in the right
 *         subtree; otherwise (the targets are on either side, or one of
 *         them equals the current node) the current node is the split
 *         point and therefore the LCA. This is O(h) with no recursion into
 *         both subtrees, unlike the general binary-tree approach below.
 *
 * Special Considerations:
 *         Only valid for a BST. A plain (non-BST) binary tree has no
 *         ordering to exploit, and needs the general recursive approach
 *         instead: recursively search both subtrees for the two targets;
 *         if a node's left and right subtree searches both return
 *         non-null, that node is the LCA (see lowestCommonAncestorGeneral).
 *
 * Inputs:
 *         root : root of the BST.
 *         p    : first target value.
 *         q    : second target value.
 *
 * Returns:
 *         Pointer to the LCA node, or NULL if root is NULL.
 *****************************************************************************/
TreeNode *lowestCommonAncestorBST(TreeNode *root, int p, int q)
{
    while (root != NULL)
    {
        if ((p < root->value) && (q < root->value))
        {
            root = root->left;
        }
        else if ((p > root->value) && (q > root->value))
        {
            root = root->right;
        }
        else
        {
            return root;
        }
    }
    return NULL;
}

/*****************************************************************************
 * Name: lowestCommonAncestorGeneral
 *
 * Description:
 *         General-tree LCA (works on any binary tree, not just a BST):
 *         recursively search the left and right subtrees for values p and
 *         q. If both subtrees report finding one of the targets, the
 *         current node is the split point where the two search paths
 *         diverge, so it's the LCA. If only one side reports a find, that
 *         result is passed back up unchanged (the LCA must be further up,
 *         or that node itself is one of p/q and an ancestor of the other).
 *
 * Inputs:
 *         root : root of the (sub)tree to search.
 *         p    : first target value.
 *         q    : second target value.
 *
 * Returns:
 *         Pointer to the LCA node, or NULL if neither target is found.
 *****************************************************************************/
TreeNode *lowestCommonAncestorGeneral(TreeNode *root, int p, int q)
{
    TreeNode *left;
    TreeNode *right;

    if ((root == NULL) || (root->value == p) || (root->value == q))
    {
        return root;
    }

    left = lowestCommonAncestorGeneral(root->left, p, q);
    right = lowestCommonAncestorGeneral(root->right, p, q);

    if ((left != NULL) && (right != NULL))
    {
        return root;
    }
    return (left != NULL) ? left : right;
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
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int size = sizeof(values) / sizeof(values[0]);
    int i;
    TreeNode *lca;

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    lca = lowestCommonAncestorBST(root, 20, 40);
    printf("BST LCA(20, 40) = %d\n", lca->value);

    lca = lowestCommonAncestorGeneral(root, 20, 40);
    printf("general LCA(20, 40) = %d\n", lca->value);

    freeTree(root);
    return 0;
}
