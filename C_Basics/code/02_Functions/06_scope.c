# include <stdio.h>
  
int x = 0;
int f()
{
   return x;
}
int g()
{
   int x = 1;
   (void)x; // shadows global x; unused here, f() reads the global instead
   return f();
}
int main()
{
  printf("%d", g());
  printf("\n");
  getchar();
}
