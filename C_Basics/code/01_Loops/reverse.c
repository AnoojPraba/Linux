#include <stdio.h>
int main()
{
    int num = 123456, ans = 0;
    while (num > 0)
    {
        int rem = num % 10;
        num /= 10;

        ans = ans * 10 + rem;
    }
    printf("Reverse is %d", ans);
    return 0;
}
