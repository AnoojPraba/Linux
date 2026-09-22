#include<stdio.h>

int main()
{
    int n;
    printf("Enter an integer\n");
    scanf("%d",&n);
    ( (n & 1) == 1 )?printf("Odd\n"):printf("Even\n");
    return 0;
}
