#include <setjmp.h>
#include <stdio.h>

// setjmp/longjmp: non-local goto, C's poor man's exception handling.
static jmp_buf env;

static void parse_level3(int bad)
{
    if (bad)
    {
        printf("  level3: error, longjmp\n");
        longjmp(env, 42);       // unwinds straight to setjmp; skips level2/1
    }
    printf("  level3: ok\n");
}

static void parse_level2(int bad)
{
    parse_level3(bad);
    printf("  level2: finished\n");   // skipped on error path
}

int main(void)
{
    // Rule: a local modified after setjmp and read after longjmp has an
    // indeterminate value unless it is volatile (registers are not restored).
    volatile int attempts = 0;
    int plain = 0;   // not volatile: value after longjmp is unspecified

    int rc = setjmp(env);       // returns 0 first time, longjmp's value after
    attempts++;
    plain++;
    if (rc == 0)
    {
        printf("first pass\n");
        parse_level2(0);
        printf("trigger error\n");
        parse_level2(1);
    }
    else
        printf("recovered via longjmp rc=%d attempts=%d\n", rc, attempts);

    (void)plain;    // reading it here would be the indeterminate-value trap
    return 0;
}
