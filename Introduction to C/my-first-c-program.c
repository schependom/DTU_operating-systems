// --- Header files ---
#include <stdio.h>

// mandatory main function
int main(int argc, char *argv[])
{

    ////////////
    // BASICS //
    ////////////

    // for-loop
    for (int i = 0; i < 10; i++)
    {
        // formatting: print integer followed by newline
        printf("%d\n", i);
    }

    // If there is an argument, print the address
    if (argc > 1) // argc is always >= 1
    {
        printf("Address of first argument: %p\n", (void *)argv[1]);
    }
    else
    {
        printf("No arguments provided.\n");
    }

    ///////////////
    // ARGUMENTS //
    ///////////////

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

    //////////////////
    // RETURN VALUE //
    //////////////////

    // return 0 to indicate successful completion
    return 0;
}