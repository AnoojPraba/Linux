#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 128

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

int height(TreeNode *root)
{
    int leftHeight;
    int rightHeight;

    if (root == NULL)
    {
        return 0;
    }

    leftHeight = height(root->left);
    rightHeight = height(root->right);

    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

// Breadth-first (level-order) traversal needs an explicit queue, unlike the
// depth-first traversals in 02_treeTraversals.c which use the call stack.
void levelOrder(TreeNode *root)
{
    TreeNode *queue[MAX_QUEUE_SIZE];
    int front = 0;
    int back = 0;

    if (root == NULL)
    {
        return;
    }

    queue[back++] = root;
    while (front < back)
    {
        TreeNode *current = queue[front++];

        printf("%d ", current->value);
        if (current->left != NULL)
        {
            queue[back++] = current->left;
        }
        if (current->right != NULL)
        {
            queue[back++] = current->right;
        }
    }
    printf("\n");
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

    printf("height: %d\n", height(root));

    printf("level order: ");
    levelOrder(root);

    freeTree(root);
    return 0;
}
