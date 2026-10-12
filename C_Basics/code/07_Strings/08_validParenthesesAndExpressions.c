// Stack-based string problems: balanced brackets, longest valid parentheses,
// minimum additions and evaluating a simple expression.

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int balanced(const char *s)
{
    char st[64];
    int top = 0;
    for (; *s; s++)
    {
        if (strchr("([{", *s)) st[top++] = *s;
        else if (strchr(")]}", *s))
        {
            if (!top) return 0;
            char open = st[--top];
            if ((*s == ')' && open != '(') || (*s == ']' && open != '[') || (*s == '}' && open != '{')) return 0;
        }
    }
    return top == 0;
}

static int longest_valid(const char *s)
{
    int st[64], top = 0, best = 0;
    st[top++] = -1;                                     /* index just before the current valid run */
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == '(') st[top++] = i;
        else
        {
            top--;
            if (top == 0) st[top++] = i;                /* unmatched ')': a new base */
            else if (i - st[top - 1] > best) best = i - st[top - 1];
        }
    }
    return best;
}

static int min_add_to_make_valid(const char *s)
{
    int open = 0, add = 0;
    for (; *s; s++)
        if (*s == '(') open++;
        else if (open) open--;
        else add++;                                     /* a ')' with nothing to close needs a '(' added */
    return open + add;
}

/* + - * / and parentheses, integers: recursive descent (precedence climbing) */
static const char *p;
static int expr(void);
static int factor(void)
{
    while (*p == ' ') p++;
    if (*p == '(') { p++; int v = expr(); while (*p == ' ') p++; p++; return v; }
    int v = 0;
    while (isdigit((unsigned char)*p)) v = v * 10 + (*p++ - '0');
    return v;
}
static int term(void)
{
    int v = factor();
    for (;;)
    {
        while (*p == ' ') p++;
        if (*p == '*') { p++; v *= factor(); }
        else if (*p == '/') { p++; v /= factor(); }
        else return v;
    }
}
static int expr(void)
{
    int v = term();
    for (;;)
    {
        while (*p == ' ') p++;
        if (*p == '+') { p++; v += term(); }
        else if (*p == '-') { p++; v -= term(); }
        else return v;
    }
}

int main(void)
{
    const char *tests[] = {"{[()]}", "([)]", "((", ""};
    for (int i = 0; i < 4; i++) printf("balanced \"%s\": %d\n", tests[i], balanced(tests[i]));
    printf("longest valid parentheses in \")()())\": %d\n", longest_valid(")()())"));
    printf("minimum additions for \"())(\": %d\n", min_add_to_make_valid("())("));
    p = "2 + 3 * (4 - 1) / 3";
    printf("2 + 3 * (4 - 1) / 3 = %d\n", expr());
    return 0;
}
