#include <stdio.h>

int main()
{
    int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int target = 5;
    int i;
    int j;
    int found = 0;

    // goto's one broadly accepted use in C: breaking out of nested loops in
    // one jump, since "break" only exits the innermost loop. Anything a
    // single well-placed goto+label can't replace with restructured control
    // flow is usually a sign the goto is being overused.
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (matrix[i][j] == target)
            {
                found = 1;
                goto searchDone;
            }
        }
    }

searchDone:
    if (found)
    {
        printf("found %d at [%d][%d]\n", target, i, j);
    }
    else
    {
        printf("%d not found\n", target);
    }

    return 0;
}
