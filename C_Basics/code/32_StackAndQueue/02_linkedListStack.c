#include <stdio.h>
#include <stdlib.h>

// Node layout mirrors the singly linked list in 33_LinkedList; a stack built
// on a linked list has no fixed capacity, unlike the array version.
typedef struct StackNode
{
    int value;
    struct StackNode *next;
} StackNode;

/*****************************************************************************
 * Name: stackIsEmpty
 *
 * Description:
 *         Checks whether the linked-list stack holds no elements.
 *
 * Inputs:
 *         top : current top node, or NULL.
 *
 * Returns:
 *         1 if empty, 0 otherwise.
 *****************************************************************************/
int stackIsEmpty(const StackNode *top)
{
    return top == NULL;
}

/*****************************************************************************
 * Name: stackPush
 *
 * Description:
 *         Pushes a new value onto the top of the linked-list stack.
 *
 * Inputs:
 *         top   : current top node, or NULL.
 *         value : value to push.
 *
 * Returns:
 *         Pointer to the new top node.
 *****************************************************************************/
StackNode *stackPush(StackNode *top, int value)
{
    StackNode *node = malloc(sizeof(StackNode));

    node->value = value;
    node->next = top;
    return node;
}

/*****************************************************************************
 * Name: stackPop
 *
 * Description:
 *         Removes the top node from the linked-list stack and frees it.
 *
 * Inputs:
 *         top   : current top node, must not be NULL.
 *         value : out-param receiving the popped value.
 *
 * Returns:
 *         Pointer to the new top node (may be NULL).
 *****************************************************************************/
StackNode *stackPop(StackNode *top, int *value)
{
    StackNode *next = top->next;

    *value = top->value;
    free(top);
    return next;
}

int main(void)
{
    StackNode *top = NULL;
    int value;

    top = stackPush(top, 10);
    top = stackPush(top, 20);
    top = stackPush(top, 30);

    while (!stackIsEmpty(top))
    {
        top = stackPop(top, &value);
        printf("popped %d\n", value);
    }

    return 0;
}
