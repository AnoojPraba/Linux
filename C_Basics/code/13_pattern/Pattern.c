#include<stdio.h>

/*

 *
 * *
 * * *
 * * * *
 * * * * *

 */
void pattern1(int lines)
{
    for (int i=1;i<=lines;i++)
    {
        for(int j=1;j<=i;j++)
            printf("* ");
    printf("\n");
    }
}

/*

 1
 1 2
 1 2 3
 1 2 3 4
 1 2 3 4 5

 */
void pattern2(int lines)
{
    for(int i=1;i<=lines;i++)
    {
        for(int j =1; j<=i;j++)
            printf("%d ",j);
        printf("\n");
    }
}

/*

 * * * * * 
 * * * * 
 * * * 
 * * 
 * 

 */
void pattern3(int n) {
    for (int row = 1; row <= n; row++)
    {
        // for every row, run the col
        for (int col = 1; col <= n-row+1; col++)
            printf("* ");
        // when one row is printed, we need to add a newline
        printf("\n");
    }
}
/*

* 
* * 
* * * 
* * * * 
* * * * * 
* * * * 
* * * 
* * 
* 

*/
void pattern4(int n) {
    for (int row = 0; row < 2 * n; row++)
    {
        int totalColsInRow = row > n ? 2 * n - row: row;
        for (int col = 0; col < totalColsInRow; col++)
            printf("* ");
        // when one row is printed, we need to add a newline
        printf("\n");
    }
}
/*
    *
   * *
  * * *
 * * * *
* * * * *
 * * * *
  * * *
   * *
    *
*/
void pattern5(int n)
{
    for (int row = 0; row < 2 * n; row++)
    {
        int totalColsInRow = row > n ? 2 * n - row: row;
        int noOfSpaces = n - totalColsInRow;
        for (int s = 0; s < noOfSpaces; s++)
            printf(" ");
        for (int col = 0; col < totalColsInRow; col++)
            printf("* ");
        printf("\n");
    }
}
/*
        1 
      2 1 2 
    3 2 1 2 3 
  4 3 2 1 2 3 4 
5 4 3 2 1 2 3 4 5 

*/
void pattern6(int n) {
    for (int row = 1; row <= n; row++) {
        for (int space = 0; space < n-row; space++) {
            printf("  ");
        }
        for (int col = row; col >= 1; col--) {
            printf("%d ",col);
        }
        for (int col = 2; col <= row; col++) {
            printf("%d ",col);
        }
        printf("\n");
    }
}
/*
        1 
      2 1 2 
    3 2 1 2 3 
  4 3 2 1 2 3 4 
5 4 3 2 1 2 3 4 5 
  4 3 2 1 2 3 4 
    3 2 1 2 3 
      2 1 2 
        1 
*/
void pattern7(int n) {
    for (int row = 1; row <= 2 * n; row++) {
        int c = row > n ? 2 * n - row: row;
        for (int space = 0; space < n-c; space++) {
            printf("  ");
        }
        for (int col = c; col >= 1; col--) {
            printf("%d ",col);
        }
        for (int col = 2; col <= c; col++) {
            printf("%d ",col);
        }
        printf("\n");
    }
}
int main()
{
    pattern7(5);
    return 0;
}
