# DTU 02159 - Operating Systems

This repository contains my notes and code examples for the course *"02159 - Operating Systems"* at DTU. It includes my own introduction to C programming, as well as various other topics related to operating systems and C.

## Author
Vincent Van Schependom
- s251739@student.dtu.dk
- vincent.vanschependom@student.kuleuven.be

## Introduction to C-programming

I wrote out this little guide with things I learned while getting to know C programming. Hope it helps you too! -Vincent

### File structure

#### Including header files

To include a `.h` headerfile, use `#INCLUDE ...`.

##### Standard C libraries

Use *angled* brackets: `<...>`, like this:
```c
#include <stdio.h>
```

Examples of standard libraries:

- `<stdio.h>`
  - 'Standard input-output'
  - `printf`, `scanf`, ...
- `<stdlib.h>`
  - 'Standard library'
  - `malloc`, `free`, `exit`, ...
- `<string.h>`
  - String manipulation functions
  - `strcpy`, `strlen`, `strcmp`, ...
- ... and many more

##### Own libraries

Use *double quotes*: `"..."`, like this: 
```c
#include "myfile.h"
```

#### Main function

```c
int main()
{

}
```

##### Exit codes

- `int` means that the function returns an integer (usually `0` for success, `1` for failure)
- `return 0;` at the end of `main` indicates that the program finished successfully

##### Arguments

```c
int main(int argc, char *argv[])
```

- `argc` 
  - = *argument <u>count</u>*
  - the **number of arguments** passed to the program
  - This **includes the program name** itself
  - So even if you don't pass any arguments, `argc` will be `1`!
- `argv` 
  - = *argument <u>vector</u>*
  - an array of character **pointers** 
    - each element of the `argv` array is a memory address 
    - this **address points to the <u>first</u> character of a string**
    - a string is a null-terminated array of characters
  - points to the actual arguments as **strings**
    - `argv[0]` is the program name
    - `argv[1]` is the first argument
    - `argv[2]` is the second argument
    - and so on...


Example

```c
myprogram hello world 123
```
- `argc` will be `4`
- `argv` will be an array of 4 character pointers (pointing to the first character of each string):
  - `argv = { "./myprogram", "hello", "world", "123" }`
  - `argv[0]` = `"./myprogram"`
  - `argv[1]` = `"hello"`
  - `argv[2]` = `"world"`
  - `argv[3]` = `"123"`

### Running our C program

#### Compiling

Compiling to a binary executable (`.out` on Linux/Mac, `.exe` on Windows):

```bash
gcc -o outputfile sourcefile.c
```

or, alternatively:

```bash
gcc sourcefile.c -o outputfile
gcc sourcefile.c -o outputfile.exe  -- on Windows
```

#### Running

Running the compiled binary:

```bash
./outputfile    -- on Linux/Mac
outputfile.exe  -- on Windows
```


### Types

#### Primitive types

- `char`
- `int`
- `float`
  - precision up to 6 digits
- `double`
  - precision up to 15 digits

#### Strings

The `string` type doesn't exist in C!
  - array of `char`
  - **null-terminated** (`'\0'` at the end)
  - `char str[] = "Hello";` is actually `{'H', 'e', 'l', 'l', 'o', '\0'}`!

See also [Arrays](#arrays).

### Data structures

#### Arrays

An array is a collection of elements of the **same type**.

##### Creation

Creating an empty array:

```c
// type array_name[array_size];
int numbers[5];
```

Creating and initializing an array:
- note the empty brackets '[]' - *size is inferred* from initializer
- also note the curly braces '{}'
```c
int numbers[] = {1, 2, 3, 4, 5};
char letters[] = {'a', 'b', 'c', 'd', 'e'};
```

##### Strings

Strings are actually just arrays of `char`. 

This will **<u>not</u>** work:
```c
string str = "Hello"; // WRONG! No 'string' type in C
```

This is/are the correct way(s) to create strings:

```c
// two ways to create a string
char str_explicit[] = {'H', 'e', 'l', 'l', 'o', '\0'};
char str_implicit[] = "Hello";
```

##### Accessing elements

```c
numbers[0] = 1; // 0-based indexing of course
numbers[1] = 2;
```

##### Getting the size of an array

1. **size is known at compile time**
    - `sizeof` operator
    - Returns the total size of the array in **bytes**. 
    - By dividing this by the size of a *single* element, you can get the number of elements in the array.
```c
int numbers[] = {1, 2, 3, 4, 5};
int size = sizeof(numbers) / sizeof(numbers[0]);

int size_in_bytes = sizeof(numbers);    // total size in bytes
int element_size = sizeof(numbers[0]);  // size of one element in bytes
```

2. **size is NOT known at compile time**
    - Use a sentinel value (like `'\0'` for strings)
    - Or pass the size as an additional argument to functions

```c
// char str[] = {'H', 'e', 'l', 'l', 'o', '\0'};
char str[] = "Hello";
int size = 0;

// Count until null-terminator (sentinel)
while (str[size] != '\0') 
// or: while (*(str + size) != '\0')
// or: while (str[size] != NULL)  -- because NULL is 0
// or: while (str[size] != 0)     -- because '\0' is 0
// or: while (str[size])          -- because '\0' is 'false'
{
    size++;
}
```

The above example of finding length with pointer arithmetic is a bit **flawed** because it relies on a **null terminator**, which **only applies to C strings**. 

For an array of integers, there is no inherent "end" marker. That is why **dynamically allocated arrays require you to keep a separate length variable**:

```c
// passing the length to a function
void myFunction(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        // ...
    }
}
```

#### Structures (`struct`)

A `struct` is a collection of variables (of **different types**) under **one name**.

##### Declaring a struct

Simple example:

```c
// Declaration without typedef
struct Person
{
    char name[50];
    int age;
    float height;
} p1{"Alice", 30, 1.62}, p2{"Bob", 25, 1.75};   // declare variables here...


// ... or declare variables later
struct Person p3 = {"Charlie", 35, 1.80};
struct Person p4 = {"Diana", 28, 1.63};
```

Adding `typedef` allows you to use `Person` instead of `struct Person` when declaring variables.

```c
// Declaration with typedef
typedef struct
{
    char name[50];
    int age;
    float height;
} Person;

// Now you can declare variables like this:
Person p1 = {"Alice", 30, 1.62};
Person p2 = {"Bob", 25, 1.75};
```

##### Accessing struct members

Use the dot operator `.` to access members of a struct variable.

```c
printf("Name: %s, Age: %d, Height: %.2f\n", p1.name, p1.age, p1.height);
```

#### Unions (`union`)

- '**member**' = variable inside a union
- **all members share the same memory location**. 
- at any given time, a union can only hold a value for **one of its members**.
- the **size** of the union is determined by the size of its **largest member**.

Example:

```c
union Data
{
    int i;
    float f;
};

union Data data;

data.i = 10;              // set integer value
printf("%d\n", data.i);   // prints 10

data.f = 220.5;           // set float value, overwriting the integer!!
printf("%f\n", data.f);   // prints 220.5

printf("%d\n", data.i);   
// UNDEFINED behavior!
// data.i is now CORRUPTED!!
```

```bash
❯ ./union
10
220.500000
1130135552
```

### Pointers (`*` and `&`)

When creating a variable, you are actually creating a space in memory to store a value.

#### What is a pointer?

A pointer is a variable that stores the **<u>address</u>** of another variable.

#### Declaring and using pointers

- *Asterisk* `*`
  - used to **declare** a pointer variable (`int *p;`)
  - used to **dereference** a pointer: get the value at the address the pointer points to (`*p`)
- *Ampersand* `&`
  - used to get the **address** of a variable (`&myInt`)

#### Double pointers

A pointer that stores the address of another pointer.\
We can **dereference double pointers multiple times** to get to the actual value.

```c
int value = 42;          // normal integer variable
int *p = &value;         // pointer to int
int **pp = &p;           // pointer to pointer (=DOUBLE POINTER) to int
printf("%d\n", **pp);    // dereference twice to get the value (42)
```

#### Generic pointers

If we want to print an address, we can use the `%p` format specifier in `printf`. We need to **cast the pointer** to `(void *)` for printing, because `%p` expects a `void *` argument.

A `void *` (as expected by `%p` in `printf`) is a **generic pointer** that can point to any data type.
Example:

```c
int myInt = 5; int *p = &myInt;               // p now holds the address of myInt
printf("Address of myInt: %p\n", (void *)p);  // cast to (void *) for printing!
```

#### Examples

##### Easy example:

```c
int myInt = 5;        // normal integer variable
int *p;               // *: creating a pointer variable (to an int)
p = &myInt;           // &: get the address of 'myInt' and store in the pointer p
printf("%d\n", *p);   // *: dereferencing p to get the value of myInt (5)
```

##### Pointer arithmetic example:

The name of an array acts as a **pointer to its first element**.

```c
int arr[] = {10, 20, 30, 40, 50};
int *p = arr;             // array name 'arr' is a pointer to the first element
printf("%d\n", *(p + 2)); // pointer arithmetic: get the 3rd element (30)
```

### IO

Make sure to include `<stdio.h>`.

#### Printing with `printf`

Formatting:
- `"%d"` or `"%i"` for `int`
- `"%c"` for `char`
- `"%f"` for `float`
- `"%lf"` for `double`
- `"%s"` for `string`
- `"%p"` for pointer (*address* in memory)!
- `"\n"` for new line
- `"\t"` for tab

Example:
```c
int a = 5;
printf("The value of a is: %d\n", a);
```

#### Getting user input with `scanf`

`scanf` takes two arguments:
1. A *format string* (like in `printf`)
2. The *<u>address</u>* of the variable to store the input in (**use `&`!**)

Example:
```c
int a;

// prompt
printf("Enter a value for a: ");

// note the '&' before 'a'!
// -> it gives the address of 'a' to scanf
// -> scanf now knows where to store the input value
scanf("%d", &a);

printf("The value of a is: %d\n", a);
```

##### The Dangers of Unread Input ⚠️

If you don't use a **length specifier** (e.g., `scanf("%s", person1.name)`), the `scanf` function will continue reading characters until it encounters whitespace. If the user's input exceeds the size of your allocated array for `person1.name` in a struct `Person`, `scanf` will write past the end of the buffer, overwriting adjacent memory. This can lead to program crashes, corrupted data, or even security vulnerabilities.

The *format specifier* `%10s` is a safeguard against this. It instructs `scanf` to read at most 10 non-whitespace characters. While it prevents a buffer overflow, **it doesn't solve the problem of leftover input.**

##### Handling Leftover Input with `fgets`

You could use `fgets` instead of `scanf` for reading strings, as it allows you to specify the maximum number of characters to read and handles leftover input more gracefully:

```c
struct Person
{
    char name[11]; // 10 characters + 1 for null terminator
};

// fgets is a safer alternative to scanf for strings
fgets(person1.name, sizeof(person1.name), stdin);

// Remove newline character if present
size_t len = strlen(person1.name);
if (len > 0 && person1.name[len - 1] == '\n')
{
    person1.name[len - 1] = '\0';
}
```

### Basic stuff

#### If-then-else, 3 ways

```c
// full-sized
if (condition)
{
    // ...
}
else if (other_condition)
{
    // ...
}
else
{
    // ...
}

// shorter (without brackets)
if (condition)              // no {} !
    // ...
else if (other_condition)   // no {} !
    // ...
else                        // no {} !
    // ...

// even shorter (ternary operator)
condition ? if_true : if_false;
```

#### Switch

```c
switch (variable)
{
    case value1:
        // ...
        break;

    case value2:
        // ...
        break;

    default:
        // ...
        break;
}
```

#### Loops

##### While loop

```c
while (condition)
{
    // ...
}
```

##### Do-while loop

```c
do
{
    // ...
} while (condition);
```

##### For loop

```c
for (initialization; condition; increment)
{
    // ...
}
```

Example:
```c
for (int i = 0; i < 10; i++)
{
    printf("%d\n", i);
}
```