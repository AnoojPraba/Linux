#include <stdio.h>

#define STACK_CAPACITY 10

/* A min-stack supports push/pop/top/getMin all in O(1) by maintaining an
 * auxiliary stack alongside the main one: every time a value is pushed onto
 * the main stack, the running minimum so far is pushed onto the aux stack
 * (either the new value, if it's smaller than the current aux top, or the
 * existing aux top otherwise). getMin() then is just an O(1) peek at the
 * aux stack's top, and popping pops both stacks in lockstep so the aux
 * stack's top always reflects the minimum of exactly what's left on the
 * main stack. */
typedef struct
{
    int data[STACK_CAPACITY];
    int minData[STACK_CAPACITY];
    int top;
} MinStack;

/*****************************************************************************
 * Name: minStackInit
 *
 * Description:
 *         Initializes a min-stack to empty.
 *
 * Inputs:
 *         stack : stack to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void minStackInit(MinStack *stack)
{
    stack->top = -1;
}

/*****************************************************************************
 * Name: minStackPush
 *
 * Description:
 *         Pushes a value onto the stack, updating the running minimum in
 *         lockstep on the auxiliary stack.
 *
 * Inputs:
 *         stack : stack to push onto.
 *         value : value to push.
 *
 * Returns:
 *         1 on success, 0 if the stack was full.
 *****************************************************************************/
int minStackPush(MinStack *stack, int value)
{
    int currentMin;

    if (stack->top == (STACK_CAPACITY - 1))
    {
        return 0;
    }

    stack->top = stack->top + 1;
    stack->data[stack->top] = value;

    if (stack->top == 0)
    {
        currentMin = value;
    }
    else if (value < stack->minData[stack->top - 1])
    {
        currentMin = value;
    }
    else
    {
        currentMin = stack->minData[stack->top - 1];
    }
    stack->minData[stack->top] = currentMin;

    return 1;
}

/*****************************************************************************
 * Name: minStackPop
 *
 * Description:
 *         Removes and returns the value at the top of the stack, discarding
 *         the corresponding auxiliary minimum entry.
 *
 * Inputs:
 *         stack : stack to pop from.
 *         value : out-param receiving the popped value.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int minStackPop(MinStack *stack, int *value)
{
    if (stack->top == -1)
    {
        return 0;
    }
    *value = stack->data[stack->top];
    stack->top = stack->top - 1;
    return 1;
}

/*****************************************************************************
 * Name: minStackGetMin
 *
 * Description:
 *         Reads the current minimum value in the stack in O(1).
 *
 * Inputs:
 *         stack : stack to query.
 *         value : out-param receiving the minimum value.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int minStackGetMin(const MinStack *stack, int *value)
{
    if (stack->top == -1)
    {
        return 0;
    }
    *value = stack->minData[stack->top];
    return 1;
}

int main(void)
{
    MinStack stack;
    int value;

    minStackInit(&stack);
    minStackPush(&stack, 5);
    minStackPush(&stack, 2);
    minStackPush(&stack, 7);
    minStackPush(&stack, 1);

    minStackGetMin(&stack, &value);
    printf("current min = %d\n", value);

    minStackPop(&stack, &value);
    printf("popped %d\n", value);

    minStackGetMin(&stack, &value);
    printf("current min = %d\n", value);

    return 0;
}
