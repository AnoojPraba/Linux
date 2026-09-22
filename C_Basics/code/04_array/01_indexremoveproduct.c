#include<stdio.h>

int main()
{
    int a[]={4, 2, 1 , 5};
    int b[4],size=4;
    /* Method 1 order(n2)
    for (int i=0;i<size;i++)
    {
        int product=1;
        for(int j=0;j<size;j++)
        {
            if(i==j)
                continue;
            product*=a[j];
        }
        b[i]=product;
    }*/
    //Method 2 orfer(2n)
    int product=1;
    for (int i=0;i<size;i++)
        product*=a[i];
    for (int i=0;i<size;i++)
        b[i]=product/a[i];
    for (int i=0;i<size;i++)
        printf("%d ",b[i]);
    return 0;
}
