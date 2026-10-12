// #error, #line, __FILE__/__LINE__/__func__, the stringify-after-expansion
// trick and a DEBUG switch.

#include <stdio.h>

#if __STDC_VERSION__ < 201112L
#error "this program needs C11 or newer"        /* stops compilation with this message */
#endif

#define STR_(x) #x
#define STR(x) STR_(x)

#line 100 "virtual_name.c"                      /* pretend we are at line 100 of another file */

int main(void)
{
    printf("file : %s\n", __FILE__);
    printf("line : %d\n", __LINE__);
    printf("func : %s\n", __func__);
    printf("line as a string literal: %s\n", STR(__LINE__));
    printf("C11 or newer: %s\n", __STDC_VERSION__ >= 201112L ? "yes" : "no");
#ifdef DEBUG
    printf("debug build\n");
#else
    printf("release build\n");
#endif
    return 0;
}
