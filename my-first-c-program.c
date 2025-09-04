// --- Header files ---
// - studio.h is included for input and output functions
//    -> 'standard input-output'
//    -> printf, scanf, ...
// - <> als het een standaard bibliotheek is
// - "" als het een eigen bestand is

#include <stdio.h>

// mandatory main function
int main(int argc, char *argv[])
{
    // for-loop
    for (int i = 0; i < 10; i++)
    {
        // formatting: print integer followed by newline
        printf("%d\n", i);
    }

    // print the number of arguments
    printf("Number of arguments: %d\n", argc);

    // number of arguments calculated by using pointer arithmetic
    int array_size = 0;
    while (*(argv + array_size) != NULL)
    {
        array_size++;
    }

    // alternative
    int array_size2 = 0;
    while (argv[array_size2])
    // You can also use: != NULL, != 0, != '\0'
    {
        array_size2++;
    }

    // print each argument
    for (int i = 0; i < argc; i++)
    {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    // return 0 to indicate successful completion
    return 0;
}