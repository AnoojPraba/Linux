#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

// STACKFUL coroutines with POSIX <ucontext.h>: each coroutine owns a separate
// stack, and swapcontext() saves the current registers and jumps to another
// context - a user-space context switch (no kernel scheduler, no preemption).
// Because each has its own stack, a coroutine can yield from ANY call depth.
// (ucontext is obsolescent in POSIX but works on glibc; boost.context, libco and
// libaco do the same with hand-written assembly. It is also how Go/fibers work
// conceptually. `swapcontext` does a sigprocmask syscall - slow vs asm switches.)
#define STACK_SIZE (64 * 1024)

static ucontext_t main_ctx, gen_ctx;
static int produced;                    // value handed from generator to main
static int gen_done;

static void yield_value(int v)          // can be called from any depth in the coroutine
{
    produced = v;
    swapcontext(&gen_ctx, &main_ctx);   // save generator, resume main
}

static void helper_that_yields(int base)    // yields from a NESTED call
{
    for (int i = 0; i < 2; i++)
        yield_value(base + i);
}

static void generator(void)
{
    for (int i = 1; i <= 3; i++)
        yield_value(i * 10);
    helper_that_yields(100);            // stackless coroutines cannot do this
    gen_done = 1;
}                                       // returning switches to uc_link (main_ctx)

int main(void)
{
    char *stack = malloc(STACK_SIZE);
    getcontext(&gen_ctx);
    gen_ctx.uc_stack.ss_sp = stack;
    gen_ctx.uc_stack.ss_size = STACK_SIZE;
    gen_ctx.uc_link = &main_ctx;        // where to go when the function returns
    makecontext(&gen_ctx, generator, 0);

    printf("values from the generator:");
    for (;;)
    {
        swapcontext(&main_ctx, &gen_ctx);   // run generator until it yields/finishes
        if (gen_done)
            break;
        printf(" %d", produced);
    }
    printf("\n");

    free(stack);
    return 0;
}
