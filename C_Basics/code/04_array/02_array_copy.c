#include<stdio.h>
#include<string.h>

int main()
{
    int a[]={4, 2, 1 , 5};
    int b[4],size=4;
    /*Method1
    for(int i=0;i<size;i++)
        b[i]=a[i];*/
    //Method2 Same order but looks clean
    memcpy(b,a,size*sizeof(int));
    for (int i=0;i<size;i++)
        printf("%d ",b[i]);
    return 0;
}
