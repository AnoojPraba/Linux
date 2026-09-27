Consider the below two examples

Example 1
    int main()
    {
        return 0;
    }
Example 2
    int main(void)
    {
        return 0;
    }

In c, example 2 is considered better as it specifies that it has to be called without any parameter
a function signature does not specify the number of arguments
the below example compiles and runs fine in c
In cpp as internal-polymorphism(checks for arguments as well) is included, hence both
fun() and fun(void) are same.

void fun() {  } 
int main(void)
{
        fun(10, "GfG", "GQ");
            return 0;
}
