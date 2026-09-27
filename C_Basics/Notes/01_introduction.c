/* Intro to c*/
C is a procedural programming language.
The main features of the C language include low-level memory access, a simple set of
keywords, and a clean style, these features make C language suitable for system
programmings like an operating system or compiler development.

/* Structure of c */
Any c program can be done in this structure only. Any other structure will cause compiler error

    Structure
        * Header
            Header file are files with extension h which contains c function declaration
            and macro definition to be shared between several source files.
        * main
            Program counter starting point for c programming. normally the return type
            of main function is int.
        * Variable declaration
            establish the variables that are used in the function.
        * body
            Operations that are performed in the function
        * return
            depends on the return type of the function.

    Example
        #include<stdio.h>           // preprocessor including. Basically includes all
                                     // standart io operations. In this example
                                     // included for printf function.
        int main()
        {                           // indicates function block starting
            printf("Hello World");  // Body of the block. Print Hello world is the
                                     // operation that is to be performed.
            return 0;               // return type of the function
        }                           // indicates function block ending

    executing a c program
        use ide or online compilers for compiling the program.
        My recommendation is to install a vm and install ubuntu and use gcc.

