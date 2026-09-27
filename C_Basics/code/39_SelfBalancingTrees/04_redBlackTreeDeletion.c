#include <stdio.h>
#include <stdlib.h>

// Node layout, colors, and rotations mirror 02_redBlackTree.c; this file
// adds deletion. Standard approach: BST delete (splicing out the node, or
// its in-order successor for the two-child case), then, if the spliced-out
// node was black, a "double-black" fixup that walks back up applying
// recoloring and rotations based on the sibling's color/children until the
// black-height invariant is restored.

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
 * Name: transplant
 *
 * Description:
 *         Replaces the subtree rooted at oldNode with the subtree rooted at
 *         newNode in the parent's child pointer (newNode may be NULL). Does
 *         not update newNode's children.
 *
 * Inputs:
 *         root    : pointer to the tree's root pointer, updated in place.
 *         oldNode : node being replaced, must not be NULL.
 *         newNode : replacement node, may be NULL.
 *
 * Returns:
 *         None
 *****************************************************************************/
void transplant(RbNode **root, RbNode *oldNode, RbNode *newNode)
{
    if (oldNode->parent == NULL)
    {
        *root = newNode;
    }
    else if (oldNode == oldNode->parent->left)
    {
        oldNode->parent->left = newNode;
    }
    else
    {
        oldNode->parent->right = newNode;
    }
    if (newNode != NULL)
    {
        newNode->parent = oldNode->parent;
    }
}

/*****************************************************************************
 * Name: minimumNode
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
RbNode *minimumNode(RbNode *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

/*****************************************************************************
 * Name: fixupDelete
 *
 * Description:
 *         Restores red-black invariants after a black node has effectively
 *         been removed, leaving fixupParent's subtree in the given child
 *         slot "double-black". doubleBlack may be NULL (representing a
 *         black NIL leaf), so the fixup is driven by fixupParent/isLeftChild
 *         rather than doubleBlack->parent. Walks up applying the four
 *         classic sibling-based cases (red sibling; black sibling with both
 *         children black; black sibling with a "near" red child; black
 *         sibling with a "far" red child), mirrored for left/right, until
 *         the extra black is absorbed or the root is reached.
 *
 * Inputs:
 *         root         : pointer to the tree's root pointer, updated in place.
 *         doubleBlack  : the node carrying the extra black, may be NULL.
 *         fixupParent  : parent of doubleBlack's slot (needed when NULL).
 *         isLeftChild  : whether doubleBlack sits in fixupParent's left slot.
 *
 * Returns:
 *         None
 *****************************************************************************/
void fixupDelete(RbNode **root, RbNode *doubleBlack, RbNode *fixupParent, int isLeftChild)
{
    while ((doubleBlack != *root) && (nodeColor(doubleBlack) == BLACK))
    {
        if (fixupParent == NULL)
        {
            break;
        }

        if (isLeftChild)
        {
            RbNode *sibling = fixupParent->right;

            if (nodeColor(sibling) == RED)
            {
                sibling->color = BLACK;
                fixupParent->color = RED;
                rotateLeft(root, fixupParent);
                sibling = fixupParent->right;
            }

            if ((nodeColor(sibling->left) == BLACK) && (nodeColor(sibling->right) == BLACK))
            {
                sibling->color = RED;
                printf("  [recolor propagate up through %d]\n", fixupParent->key);
                doubleBlack = fixupParent;
                fixupParent = doubleBlack->parent;
                isLeftChild = ((fixupParent != NULL) && (doubleBlack == fixupParent->left));
            }
            else
            {
                if (nodeColor(sibling->right) == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(root, sibling);
                    sibling = fixupParent->right;
                }
                sibling->color = fixupParent->color;
                fixupParent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(root, fixupParent);
                doubleBlack = *root;
                fixupParent = NULL;
            }
        }
        else
        {
            RbNode *sibling = fixupParent->left;

            if (nodeColor(sibling) == RED)
            {
                sibling->color = BLACK;
                fixupParent->color = RED;
                rotateRight(root, fixupParent);
                sibling = fixupParent->left;
            }

            if ((nodeColor(sibling->left) == BLACK) && (nodeColor(sibling->right) == BLACK))
            {
                sibling->color = RED;
                printf("  [recolor propagate up through %d]\n", fixupParent->key);
                doubleBlack = fixupParent;
                fixupParent = doubleBlack->parent;
                isLeftChild = ((fixupParent != NULL) && (doubleBlack == fixupParent->left));
            }
            else
            {
                if (nodeColor(sibling->left) == BLACK)
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(root, sibling);
                    sibling = fixupParent->left;
                }
                sibling->color = fixupParent->color;
                fixupParent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(root, fixupParent);
                doubleBlack = *root;
                fixupParent = NULL;
            }
        }
    }
    if (doubleBlack != NULL)
    {
        doubleBlack->color = BLACK;
    }
}

/*****************************************************************************
 * Name: rbDelete
 *
 * Description:
 *         Deletes a key from the red-black tree via standard BST deletion
 *         (swapping with the in-order successor for the two-children case),
 *         tracking the color of the node actually spliced out of the tree
 *         structure. If that color was black, calls fixupDelete to restore
 *         the black-height invariant.
 *
 * Inputs:
 *         root : pointer to the tree's root pointer, updated in place.
 *         key  : key to delete.
 *
 * Returns:
 *         None
 *****************************************************************************/
void rbDelete(RbNode **root, int key)
{
    RbNode *target = find(*root, key);
    RbNode *splicedOut;
    int splicedColor;
    RbNode *fixupChild;
    RbNode *fixupParent;
    int isLeftChild;

    if (target == NULL)
    {
        return;
    }

    if ((target->left != NULL) && (target->right != NULL))
    {
        RbNode *successor = minimumNode(target->right);

        target->key = successor->key;
        target = successor;
    }

    // target now has at most one child.
    splicedOut = target;
    splicedColor = splicedOut->color;
    fixupChild = (target->left != NULL) ? target->left : target->right;
    fixupParent = target->parent;
    isLeftChild = ((fixupParent != NULL) && (target == fixupParent->left));

    transplant(root, target, fixupChild);
    free(splicedOut);

    if (splicedColor == BLACK)
    {
        fixupDelete(root, fixupChild, fixupParent, isLeftChild);
    }
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
    // A perfectly-shaped starting tree (verified by hand-tracing its
    // preorder/color layout): 20(B) has children 10(R)/30(R); 10's children
    // are 5(B)/15(B), each with two red leaf children (1,8 and 12,18); 30's
    // children are 25(B)/35(B), each with two red leaf children (23,27 and
    // 33,40). Deletions below walk through the classic fixup cases in
    // order: red leaves (no fixup), a black leaf whose sibling is black
    // with red children (rotation case), a black leaf whose sibling is
    // black with black children (recoloring that propagates the "double
    // black" one level up), a black node with a single red child (absorbed
    // directly), and finally a two-children delete via in-order successor.
    int keys[] = { 20, 10, 30, 5, 15, 25, 35, 1, 8, 12, 18, 23, 27, 33, 40 };
    int keyCount = sizeof(keys) / sizeof(keys[0]);
    int i;

    for (i = 0; i < keyCount; i++)
    {
        rbInsert(&root, keys[i]);
    }

    printf("Initial tree, in-order with colors: ");
    inorderPrint(root);
    printf("\n");

    // 1 is a red leaf: deleting it needs no fixup at all.
    printf("\nDeleting 1 (red leaf, no fixup needed)...\n");
    rbDelete(&root, 1);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    // 8 is a red leaf too; removing it leaves 5 as a black leaf whose
    // sibling 15 is black with two red children (18's sibling 12 is red).
    printf("\nDeleting 8 (red leaf, no fixup needed)...\n");
    rbDelete(&root, 8);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    // 5 is now a black leaf; its sibling 15 is black with red children
    // (12 and 18), which drives the "black sibling with a red child"
    // rotation case in fixupDelete.
    printf("\nDeleting 5 (black leaf, sibling black with red children)...\n");
    rbDelete(&root, 5);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    // The previous rotation reshaped this subtree (12 is now red, 15 is
    // red, 18 is black). 12 is a red leaf (no fixup); 18 is then a black
    // leaf whose sibling 10 is black with black children - this drives the
    // "black sibling with black children" recolor, which turns the extra
    // black into a double-black at parent 15 and propagates the fixup one
    // level further up (see the "[recolor propagate up ...]" trace line).
    printf("\nDeleting 12 (red leaf) then 18 (triggers recolor propagation)...\n");
    rbDelete(&root, 12);
    rbDelete(&root, 18);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    // 15's only remaining child is the red node 10; deleting 15 replaces it
    // with 10 directly and, since 15 was black, the fixup immediately
    // absorbs the extra black by repainting 10 black (no further rotation
    // or propagation needed here).
    printf("\nDeleting 15 (black node with one red child, absorbed directly)...\n");
    rbDelete(&root, 15);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    printf("\nDeleting 30 (has two children, replaced by successor)...\n");
    rbDelete(&root, 30);
    printf("After: ");
    inorderPrint(root);
    printf("\n");

    found = find(root, 25);
    printf("\nfind(25) = %s\n", (found != NULL) ? "found" : "not found");
    found = find(root, 1);
    printf("find(1) = %s\n", (found != NULL) ? "found" : "not found");

    return 0;
}
