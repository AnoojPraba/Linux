#include <stdio.h>
int main(void)
{
    int* pc, c;
    c = 5;
    pc = &c;
    c = 1;
    printf("%d\n", c);    // Output: 1
    printf("%d\n", *pc);  // Ouptut: 1
    return 0;
}
