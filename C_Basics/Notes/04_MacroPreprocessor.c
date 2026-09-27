in c all lines starting from "#" are preprocessed.
Leave the below sentance incase didnt understand:
    Post syntax analyser completes, the compiler expands the preprocessor for next
    stage of compilation.
else
    after checking all the syntax is correct in the program, the compiler expands
    the preprocessor sentances for next stage of compilation

    * include directive contains header file
        angle bracket for standard folder, double quotes for current directory.
    * define is for constant or macro
        strings matching the define are replaced with the expression. (just like
        alias in linux environment)
    * Macro can take argument.
    * No evaluation done on macro as its preprocessing functionality
    * Macro can concat with ## [eg #define merge(a, b) a##b]
    * support if-else directives which are typically used for conditional compilation.
    * directives like defined, ifdef and ifndef are used to avoid redeclaration.
    * some standard macros which can be used to print program file (__FILE__), Date
      of compilation (__DATE__), Time of compilation (__TIME__) and Line Number in
      C code (__LINE__)
    *remove macro with #undef MACRO_NAME
