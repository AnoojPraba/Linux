All varuiabes are lexically scoped.
take for example

# include <stdio.h>

int x = 0;
int f()
{
   return x; //eventhough function is called from g() which has a same variable redefined
   // it sends the global variable value.
}
int g()
{
   int x = 1;
   return f(); //NOTE: x=1 is scoped with this this function and not inside f()
}
int main()
{
  printf("%d", g());
  printf("\n");
  getchar();
}

//Classification

scoping can be broadly classified into 4
*   File scope - identifiers declared outside the function
*   Block scope //self explainatory
*   function prototype scope //self explainatory
*   function scope //not predominantly used only used in labels.
