#include <stdio.h>

// STACKLESS coroutines in plain C using the switch/__LINE__ trick (Duff's
// device; the basis of Adam Dunkels' protothreads and Simon Tatham's
// "Coroutines in C"). The function remembers WHERE it was suspended in a
// state variable and `switch`es back to that point on the next call.
//
// Limits: local variables do NOT survive a yield (they live on the real stack
// that unwound) - keep state in a struct/static; no yield inside a nested
// `switch`; every yield point needs its own `case` label (__LINE__ is unique).
// Costs ~nothing: no extra stack, just an int - ideal for tiny MCUs.
#define CR_BEGIN(st) switch ((st)->line) { case 0:
#define CR_YIELD(st) do { (st)->line = __LINE__; return 1; case __LINE__:; } while (0)
#define CR_END(st)   } (st)->line = 0; return 0

struct counter { int line; int i; };            // coroutine state lives HERE

// Returns 1 while it has more to do (a value is in c->i), 0 when finished.
static int count_to(struct counter *c, int n)
{
    CR_BEGIN(c);
    for (c->i = 1; c->i <= n; c->i++)
        CR_YIELD(c);
    CR_END(c);
}

// A tiny cooperative "RTOS": a round-robin loop calls each task's step function.
struct blinker { int line; int ticks; const char *name; };

static int blink_task(struct blinker *b)
{
    CR_BEGIN(b);
    for (b->ticks = 0; b->ticks < 2; b->ticks++)
    {
        printf("  %s ON\n", b->name);
        CR_YIELD(b);                            // "sleep" until the scheduler calls again
        printf("  %s OFF\n", b->name);
        CR_YIELD(b);
    }
    CR_END(b);
}

int main(void)
{
    struct counter c = {0};
    printf("count_to(4):");
    while (count_to(&c, 4))
        printf(" %d", c.i);
    printf("\n");

    struct blinker a = { 0, 0, "LED-A" }, b = { 0, 0, "LED-B" };
    int ra = 1, rb = 1;
    printf("round-robin scheduler of two stackless tasks:\n");
    while (ra || rb)
    {
        if (ra) ra = blink_task(&a);
        if (rb) rb = blink_task(&b);
    }
    return 0;
}
