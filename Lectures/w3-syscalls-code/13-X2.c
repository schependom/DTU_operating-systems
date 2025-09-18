#include <stdio.h>

int main(void)
{
    int a;

    // Read an integer from standard input, and store it at the address of a
    scanf("%d", &a);

    // Print the value of a * 2
    printf("%d\n", a * 2);

    return 0;
}
