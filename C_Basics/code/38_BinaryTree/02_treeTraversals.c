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

// left, node, right - visits a BST's values in ascending order.
void inorder(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
}

// node, left, right - visits a node before either of its subtrees, useful
// for recreating the tree's shape (e.g. serializing it).
void preorder(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
}

// left, right, node - visits a node only after both subtrees, useful for
// safely freeing children before their parent.
void postorder(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
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

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    printf("inorder:   ");
    inorder(root);
    printf("\n");

    printf("preorder:  ");
    preorder(root);
    printf("\n");

    printf("postorder: ");
    postorder(root);
    printf("\n");

    freeTree(root);
    return 0;
}
