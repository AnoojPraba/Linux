#include <stdio.h>

// X-macro: define the list ONCE, expand it several ways. Adding a state means
// editing one line - the enum, the name table and the handler table cannot
// drift out of sync.
#define STATE_LIST(X)            \
    X(IDLE,    "idle",    0)     \
    X(RUNNING, "running", 1)     \
    X(BLOCKED, "blocked", 2)     \
    X(DONE,    "done",    3)

// Expansion 1: enum
#define X_ENUM(name, str, code) STATE_##name = code,
enum state { STATE_LIST(X_ENUM) STATE_COUNT };

// Expansion 2: string table
#define X_STR(name, str, code) [STATE_##name] = str,
static const char *state_names[] = { STATE_LIST(X_STR) };

// Expansion 3: switch cases
#define X_CASE(name, str, code) case STATE_##name: return #name;
static const char *state_ident(enum state s)
{
    switch (s)
    {
        STATE_LIST(X_CASE)
    default: return "?";
    }
}

// Stringify and token-paste basics
#define STR_(x) #x
#define STR(x) STR_(x)          // extra level so macro args expand first
#define CAT(a, b) a##b
#define VERSION 3

// do { } while (0): makes a multi-statement macro behave like one statement
// (safe inside unbraced if/else).
#define SWAP(T, a, b) do { T _t = (a); (a) = (b); (b) = _t; } while (0)

// Double evaluation hazard: MAX(i++, j++) increments twice.
#define MAX_BAD(a, b) ((a) > (b) ? (a) : (b))
// GNU statement-expression fix (GCC/Clang):
#define MAX_SAFE(a, b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); \
                          _a > _b ? _a : _b; })

int main(void)
{
    for (int s = 0; s < STATE_COUNT; s++)
        printf("%d -> %s (%s)\n", s, state_names[s], state_ident(s));

    printf("STR(VERSION)=%s STR_(VERSION)=%s\n", STR(VERSION), STR_(VERSION));

    int CAT(var, 1) = 7;
    printf("var1=%d\n", var1);

    int x = 1, y = 2;
    SWAP(int, x, y);
    printf("x=%d y=%d\n", x, y);

    int i = 5, j = 3;
    int bad = MAX_BAD(i++, j++);    // i incremented twice
    printf("MAX_BAD -> %d, i=%d (i was 5: incremented twice)\n", bad, i);
    i = 5; j = 3;
    int good = MAX_SAFE(i++, j++);
    printf("MAX_SAFE -> %d, i=%d\n", good, i);

    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return 0;
}
