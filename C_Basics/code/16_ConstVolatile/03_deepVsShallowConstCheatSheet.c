#include <stdio.h>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Cheat sheet for the four const-pointer declaration forms. Read
 *         each declaration right-to-left starting from the variable name:
 *         "p is a const pointer to an int" for `int * const p`, or
 *         "p is a pointer to a const int" for `const int *p`. For each
 *         form below, the lines that compile are shown alongside a
 *         commented-out line marked `// ERROR:` that would fail to
 *         compile if uncommented.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int x = 1;
    int y = 2;

    /* Form 1: int *p - pointer to non-const int.
     * "p is a pointer to an int". Can modify the pointer (point elsewhere)
     * AND can modify the pointee (the int it points to). */
    int *p1 = &x;

    *p1 = 10;
    p1 = &y;
    printf("Form 1: *p1 = %d\n", *p1);

    /* Form 2: const int *p (equivalently int const *p) - pointer to const
     * int. "p is a pointer to a const int". Can reassign the pointer to
     * point elsewhere, but CANNOT modify the int through this pointer. */
    const int *p2 = &x;

    p2 = &y;
    // ERROR: *p2 = 20; -- cannot modify the pointee through a pointer-to-const
    printf("Form 2: *p2 = %d\n", *p2);

    /* Form 3: int * const p - const pointer to non-const int.
     * "p is a const pointer to an int". CANNOT reassign the pointer once
     * initialized, but CAN modify the int through it. */
    int * const p3 = &x;

    *p3 = 30;
    // ERROR: p3 = &y; -- cannot reassign a const pointer
    printf("Form 3: *p3 = %d\n", *p3);

    /* Form 4: const int * const p - const pointer to const int.
     * "p is a const pointer to a const int". CANNOT reassign the pointer
     * AND CANNOT modify the int through it. */
    const int * const p4 = &x;

    // ERROR: p4 = &y; -- cannot reassign a const pointer
    // ERROR: *p4 = 40; -- cannot modify the pointee through a pointer-to-const
    printf("Form 4: *p4 = %d\n", *p4);

    return 0;
}
