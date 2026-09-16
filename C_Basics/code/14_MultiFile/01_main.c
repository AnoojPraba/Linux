#include <stdio.h>
#include "mathutils.h"

// mathutils.h declares these functions; mathutils.c defines them. The
// #ifndef/#define/#endif include guard in the header stops it from being
// processed twice if something ends up #include-ing it more than once.
int main()
{
    printf("addTwo(2, 3) = %d\n", addTwo(2, 3));
    printf("squareOf(5) = %d\n", squareOf(5));

    return 0;
}
