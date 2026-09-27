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

// Smallest value in a subtree is the leftmost node - used to find the
// in-order successor when deleting a node that has two children.
TreeNode *findMin(TreeNode *root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

// Three cases:
//   1. Leaf node - just free it, parent's link becomes NULL.
//   2. One child - splice the child up in place of the node being removed.
//   3. Two children - copy the in-order successor's value up, then
//      recursively delete the successor from the right subtree (the
//      successor itself has at most one child, so that recursion
//      terminates in case 1 or 2).
TreeNode *deleteNode(TreeNode *root, int value)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (value < root->value)
    {
        root->left = deleteNode(root->left, value);
    }
    else if (value > root->value)
    {
        root->right = deleteNode(root->right, value);
    }
    else
    {
        if ((root->left == NULL) && (root->right == NULL))
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            TreeNode *child = root->right;

            free(root);
            return child;
        }
        else if (root->right == NULL)
        {
            TreeNode *child = root->left;

            free(root);
            return child;
        }
        else
        {
            TreeNode *successor = findMin(root->right);

            root->value = successor->value;
            root->right = deleteNode(root->right, successor->value);
        }
    }

    return root;
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
    int values[] = {50, 30, 70, 20, 40, 60, 80, 65};
    int size = sizeof(values) / sizeof(values[0]);
    int i;

    for (i = 0; i < size; i++)
    {
        root = insert(root, values[i]);
    }

    // Case 1: delete a leaf (20 has no children).
    printf("before deleting leaf 20: ");
    inorder(root);
    printf("\n");
    root = deleteNode(root, 20);
    printf("after deleting leaf 20:  ");
    inorder(root);
    printf("\n\n");

    // Case 2: delete a node with one child (60 has only child 65).
    printf("before deleting one-child node 60: ");
    inorder(root);
    printf("\n");
    root = deleteNode(root, 60);
    printf("after deleting one-child node 60:  ");
    inorder(root);
    printf("\n\n");

    // Case 3: delete a node with two children (70 has children 65 and 80).
    printf("before deleting two-child node 70: ");
    inorder(root);
    printf("\n");
    root = deleteNode(root, 70);
    printf("after deleting two-child node 70:  ");
    inorder(root);
    printf("\n");

    freeTree(root);
    return 0;
}
