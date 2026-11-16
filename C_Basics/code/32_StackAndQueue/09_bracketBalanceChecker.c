#include <stdio.h>
#include <string.h>

#define STACK_CAPACITY 100

/* Each stack entry remembers the bracket and where it appeared in the string,
 * so an unclosed opener can be reported by position. */
typedef struct
{
    char bracket;
    int pos;
} StackItem;

typedef struct
{
    StackItem data[STACK_CAPACITY];
    int top;
} BracketStack;

typedef enum
{
    BRACKETS_OK = 0,
    BRACKETS_MISMATCH,   /* closer does not match the opener on top */
    BRACKETS_EXTRA_CLOSE, /* closer with nothing open */
    BRACKETS_UNCLOSED,   /* opener never closed */
    BRACKETS_TOO_DEEP    /* nesting exceeds stack capacity */
} BracketResult;

/*****************************************************************************
 * Name: stackInit
 *
 * Description:
 *         Initializes the bracket stack to empty.
 *
 * Inputs:
 *         stack : stack to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void stackInit(BracketStack *stack)
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
int stackIsEmpty(const BracketStack *stack)
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
int stackIsFull(const BracketStack *stack)
{
    return stack->top == (STACK_CAPACITY - 1);
}

/*****************************************************************************
 * Name: stackPush
 *
 * Description:
 *         Pushes an item onto the top of the stack.
 *
 * Inputs:
 *         stack : stack to push onto.
 *         item  : item to push.
 *
 * Returns:
 *         1 on success, 0 if the stack was full.
 *****************************************************************************/
int stackPush(BracketStack *stack, StackItem item)
{
    if (stackIsFull(stack))
    {
        return 0;
    }
    stack->top = stack->top + 1;
    stack->data[stack->top] = item;
    return 1;
}

/*****************************************************************************
 * Name: stackPop
 *
 * Description:
 *         Removes the item at the top of the stack.
 *
 * Inputs:
 *         stack : stack to pop from.
 *         item  : out-param receiving the popped item.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int stackPop(BracketStack *stack, StackItem *item)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }
    *item = stack->data[stack->top];
    stack->top = stack->top - 1;
    return 1;
}

/*****************************************************************************
 * Name: stackPeek
 *
 * Description:
 *         Reads the item at the top of the stack without removing it.
 *
 * Inputs:
 *         stack : stack to peek at.
 *         item  : out-param receiving the top item.
 *
 * Returns:
 *         1 on success, 0 if the stack was empty.
 *****************************************************************************/
int stackPeek(const BracketStack *stack, StackItem *item)
{
    if (stackIsEmpty(stack))
    {
        return 0;
    }
    *item = stack->data[stack->top];
    return 1;
}

/*****************************************************************************
 * Name: isOpening
 *
 * Description:
 *         Checks whether a character is an opening bracket: { [ < (.
 *
 * Inputs:
 *         c : character to test.
 *
 * Returns:
 *         1 if opening bracket, 0 otherwise.
 *****************************************************************************/
int isOpening(char c)
{
    return (c == '{') || (c == '[') || (c == '<') || (c == '(');
}

/*****************************************************************************
 * Name: isClosing
 *
 * Description:
 *         Checks whether a character is a closing bracket: } ] > ).
 *
 * Inputs:
 *         c : character to test.
 *
 * Returns:
 *         1 if closing bracket, 0 otherwise.
 *****************************************************************************/
int isClosing(char c)
{
    return (c == '}') || (c == ']') || (c == '>') || (c == ')');
}

/*****************************************************************************
 * Name: isMatchingPair
 *
 * Description:
 *         Checks whether a closing bracket matches a given opening bracket.
 *
 * Inputs:
 *         open  : opening bracket character.
 *         close : closing bracket character.
 *
 * Returns:
 *         1 if they form a matching pair, 0 otherwise.
 *****************************************************************************/
int isMatchingPair(char open, char close)
{
    return ((open == '{') && (close == '}')) ||
           ((open == '[') && (close == ']')) ||
           ((open == '<') && (close == '>')) ||
           ((open == '(') && (close == ')'));
}

/*****************************************************************************
 * Name: checkBrackets
 *
 * Description:
 *         Verifies that every bracket in the string is opened and closed
 *         properly and in the right order. Openers are pushed on a stack;
 *         each closer must match the opener on top. Characters that are not
 *         brackets are ignored. The string is balanced iff no closer fails
 *         to match and the stack is empty at the end.
 *
 * Inputs:
 *         str    : null-terminated string to check.
 *         errPos : out-param receiving the index of the offending character
 *                  (the bad closer, or the unclosed opener); set to -1 when
 *                  the string is balanced.
 *
 * Returns:
 *         BRACKETS_OK if balanced, otherwise the kind of failure.
 *****************************************************************************/
BracketResult checkBrackets(const char *str, int *errPos)
{
    BracketStack stack;
    StackItem item;
    int i;

    stackInit(&stack);
    *errPos = -1;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (isOpening(str[i]))
        {
            item.bracket = str[i];
            item.pos = i;
            if (!stackPush(&stack, item))
            {
                *errPos = i;
                return BRACKETS_TOO_DEEP;
            }
        }
        else if (isClosing(str[i]))
        {
            if (!stackPop(&stack, &item))
            {
                *errPos = i;
                return BRACKETS_EXTRA_CLOSE;
            }
            if (!isMatchingPair(item.bracket, str[i]))
            {
                *errPos = i;
                return BRACKETS_MISMATCH;
            }
        }
    }

    if (stackPeek(&stack, &item))
    {
        *errPos = item.pos;
        return BRACKETS_UNCLOSED;
    }

    return BRACKETS_OK;
}

/*****************************************************************************
 * Name: resultText
 *
 * Description:
 *         Converts a BracketResult to a printable description.
 *
 * Inputs:
 *         result : result code from checkBrackets.
 *
 * Returns:
 *         Constant string describing the result.
 *****************************************************************************/
const char *resultText(BracketResult result)
{
    switch (result)
    {
    case BRACKETS_OK:
        return "balanced";
    case BRACKETS_MISMATCH:
        return "closing bracket does not match the last opened one";
    case BRACKETS_EXTRA_CLOSE:
        return "closing bracket with nothing open";
    case BRACKETS_UNCLOSED:
        return "opening bracket never closed";
    case BRACKETS_TOO_DEEP:
        return "nesting too deep for the stack";
    }
    return "unknown";
}

int main(int argc, char *argv[])
{
    const char *tests[] = {"{[<>]}", "a<b[c]>{d}", "x1<2[34]5>{6} end 7", "12<3{4>5}", "9>", "<<8", "{[}", "([)]", ""};
    int count = sizeof(tests) / sizeof(tests[0]);
    BracketResult result;
    int errPos;
    int i;

    if (argc > 1)
    {
        for (i = 1; i < argc; i++)
        {
            result = checkBrackets(argv[i], &errPos);
            printf("\"%s\" -> %s", argv[i], resultText(result));
            if (result != BRACKETS_OK)
            {
                printf(" (at index %d)", errPos);
            }
            printf("\n");
        }
        return 0;
    }

    for (i = 0; i < count; i++)
    {
        result = checkBrackets(tests[i], &errPos);
        printf("\"%s\" -> %s", tests[i], resultText(result));
        if (result != BRACKETS_OK)
        {
            printf(" (at index %d)", errPos);
        }
        printf("\n");
    }

    return 0;
}
