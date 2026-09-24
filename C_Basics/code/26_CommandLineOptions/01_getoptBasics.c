#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int opt;
    int verbose = 0;
    const char *outputFile = NULL;

    // getopt walks argv looking for recognized single-character flags -
    // "v" takes no argument, "o:" requires one (the colon is what marks
    // that), matching the raw argc/argv handling in
    // 18_CommandLineArgs/01_argcArgv.c but with flag parsing done for you.
    while ((opt = getopt(argc, argv, "vo:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = 1;
                break;

            case 'o':
                // optarg is set by getopt to point at the argument
                // following an option that requires one (here, -o's value).
                outputFile = optarg;
                break;

            case '?':
                printf("unknown option encountered\n");
                break;

            default:
                break;
        }
    }

    printf("verbose = %d\n", verbose);
    printf("outputFile = %s\n", (outputFile != NULL) ? outputFile : "(none)");

    // optind is left pointing at the first non-option argument - anything
    // after the recognized flags, e.g. positional filenames.
    printf("first positional arg index = %d\n", optind);

    return 0;
}
