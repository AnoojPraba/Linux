#include<stdio.h>

int main()
{
    int n=10;
    int a=0,b=1;
    int count = 2;
    while (count <= n) {
        int temp = b;
        b = b + a;
        a = temp;
        count++;
    }
    printf("Val = %d",b);
    return 1;

}
