#include <stdio.h>

#define STACK_CAPACITY 5

typedef struct
{
    int data[STACK_CAPACITY];
    int top;
} ArrayStack;

/*****************************************************************************
 * Name: stackInit
 *
 * Description:
 *         Initializes an array-based stack to empty.
 *
 * Inputs:
 *         stack : stack to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void stackInit(ArrayStack *stack)
{
    stack->top = -1;
}

/*****************************************************************************
 * Name: stackIsEmpty
 *
 * Description:
 *         Checks whether the stack holds no elements.
 *
 * Inputs:
 *         stack : stack to check.
 *
 * Returns:
 *         1 if empty, 0 otherwise.
 *****************************************************************************/
int stackIsEmpty(const ArrayStack *stack)
{
    return stack->top == -1;
}

/*****************************************************************************
 * Name: stackIsFull
 *
 * Description:
 *         Checks whether the stack has reached its fixed capacity.
 *
 * Inputs:
 *         stack : stack to check.
 *
 * Returns:
 *         1 if full, 0 otherwise.
 *****************************************************************************/
int stackIsFull(const ArrayStack *stack)
{
    return stack->top == (STACK_CAPACITY - 1);
}

/*****************************************************************************
 * Name: stackPush
 *
 * Description:
 *         Pushes a value onto the top of the stack.
 *
 * Inputs:
 *         stack : stack to push onto.
 *         value : value to push.
 *
 * Returns:
 *         1 on success, 0 if the stack was full.
 *****************************************************************************/
int stackPush(ArrayStack *stack, int value)
{
    if (stackIsFull(stack))
    {
        return 0;
    }
    stack->top = stack->top + 1;
    stack->data[stack->top] = value;
    return 1;
}

/*****************************************************************************
 * Name: stackPop
 *
 * Description:
 *         Removes and returns the value at the top of the stack.
 *
 * Inputs:
 *         stack : stack to pop from.
 *         value : out-param receiving the popped value.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int stackPop(ArrayStack *stack, int *value)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }
    *value = stack->data[stack->top];
    stack->top = stack->top - 1;
    return 1;
}

/*****************************************************************************
 * Name: stackPeek
 *
 * Description:
 *         Reads the value at the top of the stack without removing it.
 *
 * Inputs:
 *         stack : stack to peek at.
 *         value : out-param receiving the top value.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int stackPeek(const ArrayStack *stack, int *value)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }
    *value = stack->data[stack->top];
    return 1;
}

int main(void)
{
    ArrayStack stack;
    int value;
    int i;

    stackInit(&stack);
    for (i = 1; i <= STACK_CAPACITY; i++)
    {
        stackPush(&stack, i * 10);
    }

    if (!stackPush(&stack, 999))
    {
        printf("push rejected: stack full\n");
    }

    while (stackPop(&stack, &value))
    {
        printf("popped %d\n", value);
    }

    return 0;
}
