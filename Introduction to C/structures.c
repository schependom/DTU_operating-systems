#include <stdio.h>

int main(int argc, char *argv[])
{
    // ------------------------------

    // Define a structure to hold a point in 2D space
    struct Point
    {
        int x;
        int y;
    };

    // Create and initialize a Point instance
    struct Point p1;
    p1.x = 10;
    p1.y = 20;

    // Print the coordinates of the point
    printf("Point p1: (%d, %d)\n", p1.x, p1.y);

    // Create and initialize another Point instance using a compound literal
    struct Point p2 = {30, 40};
    printf("Point p2: (%d, %d)\n", p2.x, p2.y);

    // ------------------------------

    // Define a structure with typedef for easier usage
    typedef struct
    {
        char name[50];
        int age;
    } Person;

    // Create and initialize a Person instance
    Person person1;
    printf("Enter name: ");
    scanf("%10s", person1.name); // Limit input!! to avoid buffer overflow
    printf("Enter age: ");
    scanf("%d", &person1.age); // address with &!!

    // Return 0 to indicate successful completion
    return 0;
}