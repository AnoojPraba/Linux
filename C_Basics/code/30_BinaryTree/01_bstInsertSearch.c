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

// A BST keeps every left-subtree value smaller and every right-subtree value
// larger than the node's own value - insert must return the (possibly new)
// subtree root, so the caller can reattach it.
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

TreeNode *search(TreeNode *root, int value)
{
    if ((root == NULL) || (root->value == value))
    {
        return root;
    }

    if (value < root->value)
    {
        return search(root->left, value);
    }
    return search(root->right, value);
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

int main()
{
    TreeNode *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int size = sizeof(values) / sizeof(values[0]);
    int i;
    int target = 60;
    TreeNode *found;

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    found = search(root, target);
    if (found != NULL)
    {
        printf("found %d in tree\n", found->value);
    }
    else
    {
        printf("%d not found\n", target);
    }

    freeTree(root);
    return 0;
}
