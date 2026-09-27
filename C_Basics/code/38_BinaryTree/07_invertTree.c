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
 * Name: invertTree
 *
 * Description:
 *         Recursively swaps every node's left and right children, mirroring
 *         the whole tree. The famous "invert a binary tree" interview
 *         question - simple to state, and it tests basic comfort with tree
 *         recursion (swap, then recurse into both now-swapped children).
 *
 * Inputs:
 *         root : root of the (sub)tree to invert.
 *
 * Returns:
 *         Pointer to the (unchanged) root, now inverted.
 *****************************************************************************/
TreeNode *invertTree(TreeNode *root)
{
    TreeNode *temp;

    if (root == NULL)
    {
        return NULL;
    }

    temp = root->left;
    root->left = root->right;
    root->right = temp;

    invertTree(root->left);
    invertTree(root->right);

    return root;
}

void printInOrder(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    printInOrder(root->left);
    printf("%d ", root->value);
    printInOrder(root->right);
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

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    printf("in-order before invert: ");
    printInOrder(root);
    printf("\n");

    invertTree(root);

    printf("in-order after invert:  ");
    printInOrder(root);
    printf("\n");

    freeTree(root);
    return 0;
}
