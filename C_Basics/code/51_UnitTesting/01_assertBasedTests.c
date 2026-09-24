#include <stdio.h>

// Real C unit test frameworks (CUnit, Check, Unity) add test discovery,
// setup/teardown, and structured reporting on top of this exact core
// idea: call a function, compare its result to an expected value, record
// pass/fail, keep going instead of aborting on the first failure (unlike
// a raw assert() from 20_ControlFlowExtras/02_assert.c, which halts the
// whole program on the first violation - fine for an invariant, wrong for
// a test suite that should report every failure in one run).
static int testsRun = 0;
static int testsFailed = 0;

#define EXPECT_EQ(actual, expected, testName) \
    do { \
        testsRun++; \
        if ((actual) != (expected)) \
        { \
            testsFailed++; \
            printf("FAIL: %s (expected %d, got %d)\n", testName, \
                   (int)(expected), (int)(actual)); \
        } \
        else \
        { \
            printf("PASS: %s\n", testName); \
        } \
    } while (0)

int add(int a, int b)
{
    return a + b;
}

int factorial(int n)
{
    if (n <= 1)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

// A deliberately wrong implementation, to show what a failing test
// actually looks like in the output below - not every function under
// test is assumed correct just because it compiles.
int buggyMax(int a, int b)
{
    return a;
}

void testAdd(void)
{
    EXPECT_EQ(add(2, 3), 5, "add(2, 3) == 5");
    EXPECT_EQ(add(-1, 1), 0, "add(-1, 1) == 0");
}

void testFactorial(void)
{
    EXPECT_EQ(factorial(0), 1, "factorial(0) == 1");
    EXPECT_EQ(factorial(5), 120, "factorial(5) == 120");
}

void testBuggyMax(void)
{
    EXPECT_EQ(buggyMax(3, 7), 7, "buggyMax(3, 7) == 7");
}

int main()
{
    testAdd();
    testFactorial();
    testBuggyMax();

    printf("\n%d/%d tests passed\n", testsRun - testsFailed, testsRun);

    // Returning nonzero on failure is what lets a CI pipeline treat a
    // failing test suite as a failing build step, the same way -Werror
    // (mentioned in Notes/26_SeniorCInterviewQuestions.c) turns a warning
    // into a hard build failure instead of something easy to ignore.
    return (testsFailed > 0) ? 1 : 0;
}
