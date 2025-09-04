#include <stdio.h>

int main(int argc, char *argv[])
{

    // Define a union to hold EITHER an integer or a float
    //      -> 'Member' = variable inside a union
    //      -> All members share the same memory location
    //      -> Size of union is size of largest member
    //      -> Can only use one member (int or float) at a time!!

    union Data
    {
        int i;
        float f;
    };

    // Create a union instance
    union Data data;

    data.i = 10;            // set integer value
    printf("%d\n", data.i); // prints 10

    data.f = 220.5;         // set float value, overwriting the integer!!
    printf("%f\n", data.f); // prints 220.5

    printf("%f\n", data.i); // undefined behavior! data.i is now corrupted
}