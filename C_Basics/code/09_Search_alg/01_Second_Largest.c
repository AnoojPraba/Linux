#include<stdio.h>

int main()
{
    int array[]={11, 20, 15, 13, 10, 19};
    int size=5;
    int max, sec_max;
    max=sec_max=array[0];
    for (int i=1; i <=size; i ++)
    {
        if (array[i] > max)
        {
            //change max and sec_max
            sec_max=max;
            max=array[i];
        }
        else  if (array[i] >sec_max)
        {
            //change sec_max value
            sec_max=array[i];
        }

    }
    // Print the sec_max
    printf(" Second max in an array is %d", sec_max);
    return 0;
}
