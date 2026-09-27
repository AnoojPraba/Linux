Compilation basically means converting the code into executable code.

//Steps of compilation
    *   Preprocessing
            remove comments,Macro Expansion, include file,conditional compilation are done here
            input file: filename.c
            output file: filename.i
    *   Compilation
            Convert the preprocessed file into assembly level instructions.
            input file: filename.i
            output file: filename.s
    *   Assembly
            Convert the assembly level instructions to machine level instructions.
            input file:filename.s
            output file: filename.o
    *   Linking
            Linking all the function calls with definitions.
            does some extra work to add some extra code required when the program starts and ends
            input file: filename.o
            output file: filename
//Temp files
On a normal environment all the temp files will be deleted.
Incase we want to store all the files,
        $gcc -Wall -save-temps filename.c –o filename
Will save all the temp files
filename with extentions i and s (preprocessed file and compilation files) can be read
with vim editor
we can check the size utilized using size command followed by the .o file or output
application to check the memory used.
