#include <stdio.h>

#define MAX_LEN 100

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
    if ((open == '(') && (close == ')'))
    {
        return 1;
    }
    if ((open == '{') && (close == '}'))
    {
        return 1;
    }
    if ((open == '[') && (close == ']'))
    {
        return 1;
    }
    return 0;
}

/*****************************************************************************
 * Name: isValidParentheses
 *
 * Description:
 *         Determines whether a string of brackets is balanced and properly
 *         nested, using a stack: every opening bracket is pushed, and every
 *         closing bracket must match the bracket currently on top of the
 *         stack (pop-and-match). The string is valid iff every closing
 *         bracket found a match and the stack is empty once the whole
 *         string has been consumed - a non-empty stack at the end means
 *         some opening bracket was never closed.
 *
 * Inputs:
 *         str : null-terminated string containing only '(', ')', '{', '}',
 *               '[', ']'.
 *
 * Returns:
 *         1 if the brackets are valid, 0 otherwise.
 *****************************************************************************/
int isValidParentheses(const char *str)
{
    char stack[MAX_LEN];
    int top;
    int i;
    char c;

    top = -1;

    for (i = 0; str[i] != '\0'; i++)
    {
        c = str[i];

        if ((c == '(') || (c == '{') || (c == '['))
        {
            top = top + 1;
            stack[top] = c;
        }
        else
        {
            if ((top == -1) || (!isMatchingPair(stack[top], c)))
            {
                return 0;
            }
            top = top - 1;
        }
    }

    return top == -1;
}

int main(void)
{
    const char *tests[] = {"()[]{}", "(]", "([)]", "{[]}", "((("};
    int count = sizeof(tests) / sizeof(tests[0]);
    int i;

    for (i = 0; i < count; i++)
    {
        printf("\"%s\" -> %s\n", tests[i], isValidParentheses(tests[i]) ? "valid" : "invalid");
    }

    return 0;
}
