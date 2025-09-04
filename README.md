# DTU 02159 - Operating Systems

This repository contains my notes and code examples for the course _"02159 - Operating Systems"_ at DTU. It includes my own introduction to C programming, as well as various other topics related to operating systems and C.

## Author

Vincent Van Schependom

-   s251739@student.dtu.dk
-   vincent.vanschependom@student.kuleuven.be

## Introduction to C-programming

I wrote out this little guide with things I learned while getting to know C programming. Hope it helps you too! -Vincent

### C in a nutshell

C is a language that is:

<details>
<summary>static</summary>

-   compiled
-   types are checked at compile time
-   variables must be declared before use
</details>

<details>
<summary>procedural</summary>

-   functions
-   no classes/objects
</details>

<details>
<summary>typed</summary>

-   primitive types: `int`, `char`, `float`, `double`
-   derived types: `array`, `struct`, `union`, `pointer`
-   no type inference

</details>

<details>
<summary>low-level</summary>

-   close to hardware
-   manual memory management
-   pointers

</details>

<details>
<summary>structured</summary>

-   code is organized into functions
-   use of control structures (if, for, while)
</details>

<details>
<summary>imperative</summary>

-   focus on how to perform tasks
-   step-by-step instructions

</details>

### File structure

#### Including header files

To include a `.h` headerfile, use `#INCLUDE ...`.

##### Standard C libraries

Use _angled_ brackets: `<...>`, like this:

```c
#include <stdio.h>
```

Examples of standard libraries:

-   `<stdio.h>`
    -   'Standard input-output'
    -   `printf`, `scanf`, ...
-   `<stdlib.h>`
    -   'Standard library'
    -   `malloc`, `free`, `exit`, ...
-   `<string.h>`
    -   String manipulation functions
    -   `strcpy`, `strlen`, `strcmp`, ...
-   ... and many more

##### Own libraries

Use _double quotes_: `"..."`, like this:

```c
#include "myfile.h"
```

#### Other Preprocessor directives

We just saw `#include`, but there are more so-called **preprocessor directives**:

-   `#define`
    -   Used to define constants or macros
    -   Example:
        ```c
        #define PI 3.14159            // can be constants
        #define SQUARE(x) ((x) * (x)) // can be function-like
        ```
-   `#ifdef`, `#ifndef`, `#endif` (conditional preprocessor macros)
    -   Used for conditional compilation
    -   Example in `.c`-files:
        ```c
        #ifdef debug
            printf("Debug mode is on\n");
        #endif
        ```
    -   The gcc option `-Dname=value` sets a preprocessor define
        that can be used. Example:
        ```bash
        gcc -Ddebug myprogram.c -o myprogram
        ```
-   `#pragma`
    -   Used to provide special instructions to the compiler
    -   Examples:
        ```c
        #pragma once                // ensures the header file is included only once
        #pragma GCC optimize ("O3") // optimize for speed
        ```

#### Main function

```c
// Basic
int main()
{

}
```

##### Exit codes

-   `int` means that the function returns an integer (usually `0` for success, `1` for failure)
-   `return 0;` at the end of `main` indicates that the program finished successfully

##### Arguments

```c
int main(int argc, char *argv[]) {
    // Your code here
}
```

-   `argc`
    -   = _argument <u>count</u>_
    -   the **number of arguments** passed to the program
    -   This **includes the program name** itself
    -   So even if you don't pass any arguments, `argc` will be `1`!
-   `argv`
    -   = _argument <u>vector</u>_
    -   an array of character **pointers**
        -   each element of the `argv` array is a memory address
        -   this **address points to the <u>first</u> character of a string**
        -   a string is a null-terminated array of characters
    -   points to the actual arguments as **strings**
        -   `argv[0]` is the program name
        -   `argv[1]` is the first argument
        -   `argv[2]` is the second argument
        -   and so on...

Example

```c
myprogram hello world 123
```

-   `argc` will be `4`
-   `argv` will be an array of 4 character pointers (pointing to the first character of each string):
    -   `argv = { "./myprogram", "hello", "world", "123" }`
    -   `argv[0]` = `"./myprogram"`
    -   `argv[1]` = `"hello"`
    -   `argv[2]` = `"world"`
    -   `argv[3]` = `"123"`

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

-   `char`
-   `int`
-   `float`
    -   precision up to 6 digits
-   `double`
    -   precision up to 15 digits

#### Strings

The `string` type doesn't exist in C!

-   array of `char`
-   **null-terminated** (`'\0'` at the end)
-   `char str[] = "Hello";` is actually `{'H', 'e', 'l', 'l', 'o', '\0'}`!

See also [Arrays](#arrays).

### Basic stuff

#### Bitwise operators

-   `AND` is true only if both operands are true.
-   `OR` is true if any operand is true.
-   `XOR` is true if only one of the operand is true.

| Operator | Meaning     | Examples                             | Value                |
| -------- | ----------- | ------------------------------------ | -------------------- |
| `&`      | AND         | `0x77 & 0x3;` <br> `0x77 & 0x0;`     | `0x3` <br> `0`       |
| `\|`     | OR          | `0x700 \| 0x33;` <br> `0x070 \| 0;`  | `0x733` <br> `0x070` |
| `^`      | XOR         | `0x770 ^ 0x773;` <br> `0x33 ^ 0x33;` | `0x3` <br> `0`       |
| `<<`     | Left shift  | `0x01 << 4;` <br> `1 << 2;`          | `0x10` <br> `4`      |
| `>>`     | Right shift | `0x010 >> 4;` <br> `4 >> 1;`         | `0x01` <br> `2`      |

=/= logical operators, like `&&`, `||` and `!`

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

-   note the empty brackets '[]' - _size is inferred_ from initializer
-   also note the curly braces '{}'

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
    - By dividing this by the size of a _single_ element, you can get the number of elements in the array.

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

#### Enums (`enum`)

An `enum` is a user-defined type that consists of a set of named integer constants, called enumerators.

Example:

```c
enum Day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
enum Day today = WEDNESDAY;

printf("Day number: %d\n", today);  // prints 2
printf("Day number: %d\n", FRIDAY); // prints 4
printf("Day: %s\n", (today == WEDNESDAY) ? "Wednesday" : "Not Wednesday");
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

##### Structures and functions

By default, structs are passed to functions **by value** (a **copy** is made).

```c
// Structures are copied element-wise (pass by value)
void printPerson(Person p)
{
    printf("Name: %s, Age: %d, Height: %.2f\n", p.name, p.age, p.height);
}
```

For large structs, it's more efficient to pass a **pointer** to the struct (pass by reference).

```c
// Pass a pointer to the struct (pass by reference)
void printPerson(Person *p)
{
    printf("Name: %s, Age: %d, Height: %.2f\n", p->name, p->age, p->height);
    // use '->' to access members via pointer!
}
```

#### Unions (`union`)

-   '**member**' = variable inside a union
-   **all members share the same memory location**.
-   at any given time, a union can only hold a value for **one of its members**.
-   the **size** of the union is determined by the size of its **largest member**.

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

-   _Asterisk_ `*`
    -   used to **declare** a pointer variable (`int *p;`)
    -   used to **dereference** a pointer: get the value at the address the pointer points to (`*p`)
-   _Ampersand_ `&`
    -   used to get the **address** of a variable (`&myInt`)

#### Passing pointers to functions

Passing a pointer to a function allows the function to modify the original variable (**pass by reference**).

```c
void increment(int *p)
{
    (*p)++; // dereference p to get the value, then increment it
}
```

This can be useful for:

1. **Returning multiple values** from a function
    ```c
    void getMinMax(int arr[], int size, int *min, int *max)
    {
        *min = arr[0]; // dereference min to store the minimum value
        *max = arr[0]; // dereference max to store the maximum value
        for (int i = 1; i < size; i++)
        {
            if (arr[i] < *min) *min = arr[i];
            if (arr[i] > *max) *max = arr[i];
        }
    }
    ```
2. **Modifying large data structures** without copying them
    - Use `->` instead of `.`
    - Example (where `p->age` is equivalent to `(*p).age`):
        ```c
        void updatePerson(Person *p)
        {
            p->age += 1; // use the '->' operator to access members via pointer
        }
        ```

#### Double pointers

A pointer that stores the address of another pointer.\
We can **dereference double pointers multiple times** to get to the actual value.

```c
int value = 42;          // normal integer variable
int *p = &value;         // pointer to int
int **pp = &p;           // pointer to pointer (=DOUBLE POINTER) to int
printf("%d\n", **pp);    // dereference twice to get the value (42)
```

#### Casting pointers

We can explicitly cast any pointer type to any other pointer
type:

```c
double *p; int *pn;
p = (double *)pn;
```

##### Use-case: printing addresses

If we want to print an address, we can use the `%p` format specifier in `printf`. We need to **cast the pointer** to `(void *)` for printing, because `%p` expects a `void *` argument.

A `void *` (as expected by `%p` in `printf`) is a **generic pointer** that can point to any data type.
Example:

```c
int myInt = 5; int *p = &myInt;               // p now holds the address of myInt
printf("Address of myInt: %p\n", (void *)p);  // cast to (void *) for printing!
```

##### Dangers of casting pointers

Can cause segmentation faults if you dereference a pointer of the wrong type. See also [Segmentation faults](#segmentation-fault-segfault).

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

-   `"%d"` or `"%i"` for `int`
-   `"%c"` for `char`
-   `"%f"` for `float`
-   `"%lf"` for `double`
-   `"%s"` for `string`
-   `"%p"` for pointer (_address_ in memory)!
-   `"\n"` for new line
-   `"\t"` for tab

Example:

```c
int a = 5;
printf("The value of a is: %d\n", a);
```

#### Getting user input with `scanf`

`scanf` takes two arguments:

1. A _format string_ (like in `printf`)
2. The _<u>address</u>_ of the variable to store the input in (**use `&`!**)

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

The _format specifier_ `%10s` is a safeguard against this. It instructs `scanf` to read at most 10 non-whitespace characters. While it prevents a buffer overflow, **it doesn't solve the problem of leftover input.**

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

### Memory Allocation

#### 1. `malloc()`

```c
ptr = (type *)malloc(size_in_bytes);
```

-   Purpose: Allocate a block of memory of specified size.
-   Content: **Uninitialized** (garbage values).
-   Use when: You know how many bytes you need and want a single memory block.

Example:

```c
int *arr = (int *)malloc(10 * sizeof(int)); // allocate array of 10 ints
```

---

#### 2. `calloc()`

```c
ptr = (type *)calloc(num_elements, size_of_element);
```

-   Purpose: Allocate memory for an **array** and initialize all bytes to 0.
-   Use when: You need **zero-initialized** memory for an array.

Example:

```c
int *arr = (int *)calloc(10, sizeof(int)); // 10 ints, initialized to 0
```

---

#### 3. `realloc()`

```c
ptr = (type *)realloc(ptr, new_size_in_bytes);
```

-   Purpose: Resize a previously allocated memory block.
-   Use when: You want to expand or shrink a dynamic array while preserving existing data.

Example:

```c
arr = (int *)realloc(arr, 20 * sizeof(int)); // resize array to 20 ints
```

---

#### 4. `free()`

```c
free(ptr);
```

-   Purpose: Deallocate memory previously allocated with malloc(), calloc(), or realloc().
-   Use when: You are done using dynamically allocated memory to avoid memory leaks.

Example:

```c
free(arr);
arr = NULL; // prevent dangling pointer
```

---

#### Best Practices

-   Pair every malloc(), calloc(), or realloc() with free().
-   Never free memory not dynamically allocated (e.g., local variables).
-   Set pointers to NULL after freeing to avoid dangling pointers.

### Errors

#### Segmentation fault (segfault)

A **segmentation fault (segfault)** occurs when a program tries to access memory it is not allowed to.

1. **Dereferencing invalid pointers**

    - Using a pointer that hasn’t been initialized
        ```c
        int *p;
        *p = 10; // invalid, p is uninitialized
        ```
    - Dereferencing `NULL` or arbitrary addresses
    - Using a pointer after freeing it
        ```c
        free(p);
        *p = 42; // use-after-free
        ```

2. **Out-of-bounds array access**

    - Accessing elements outside the array’s valid range
        ```c
        int arr[3];
        arr[3] = 10; // invalid index (0–2)
        ```

3. **Stack overflow**

    - Excessive recursion or very large local arrays
        ```c
        void recurse() { recurse(); }
        int big[100000000]; // too large for stack
        ```

4. **Mismatched pointer types**

    - Accessing memory through a pointer cast to the wrong type
        ```c
        int x = 10;
        char *p = (char*)&x;
        p[100] = 'a'; // may access invalid memory
        ```

5. **Accessing freed or unallocated heap memory**
    - Using memory without allocating it first
        ```c
        int *p;
        *p = 5; // no malloc
        ```
    - Using memory after `free`
        ```c
        int *p = malloc(sizeof(int));
        free(p);
        *p = 5; // use-after-free
        ```

## Memory Management

### Virtual Memory

Virtual memory is an abstraction: it maps program addresses to regions of **physical memory** (RAM or disk).

### Key Usable Regions

-   **Stack**

    -   Stores **local variables** declared inside functions
    -   Grows and shrinks automatically as functions are called and return

-   **Heap**
    -   Stores **dynamically allocated memory** (e.g., via `malloc` in C)
    -   Manually managed: you must explicitly allocate and free memory

### 32-bit Systems

On a **32-bit operating system**, the total virtual memory address space is **limited to 4 GiB**.

-   **Windows (32-bit)**: typically ~2 GiB usable per process
-   **Linux (32-bit)**: typically ~3–4 GiB usable per process

#### Why 4 GiB on 32-bit?

A **32-bit CPU** uses 32-bit pointers (memory addresses).

-   A 32-bit number can represent at most `2^32 = 4,294,967,296` unique addresses.
-   That equals **4 GiB of addressable memory**.
-   Some of this space is reserved for the kernel and hardware mappings, so user programs usually get less.

### 64-bit Systems

On **64-bit systems**, the virtual memory space is vastly larger (terabytes to exabytes in theory), so the 4 GiB limit does not apply.

## Little- vs Big-endian

![Endianness](Images/endianness.jpg?raw=true "Endianness (Little vs Big Endian)")

Denote:

-   MSB = _Most Significant <u>Byte</u>_ (the "big end")
-   LSB = _Least Significant <u>Byte</u>_ (the "little end").

We use the following 4-byte (32-bit) hexadecimal number as an example: `0x87654321`:

| MSB  |      |      | LSB  |
| ---- | ---- | ---- | ---- |
| 0x87 | 0x65 | 0x43 | 0x21 |

Depending on which byte (the MSB or the LSB) is stored at the **<u>lowest</u>** memory address, we distinguish between Big Endian and Little Endian systems.

### Big Endian

-   The **<u>M</u>SB** is stored at the lowest memory address.
-   **Network Order**: the standard byte order for network protocols is Big Endian.

| Address | Value                  |
| ------- | ---------------------- |
| 0x00    | **0x87** (=<u>M</u>SB) |
| 0x01    | 0x65                   |
| 0x02    | 0x43                   |
| 0x03    | 0x21                   |

### Little Endian

-   The **<u>L</u>SB** is stored at the lowest memory address.
-   **Clients**: Most modern systems (like x86 and ARM) use Little Endian.

| Address | Value                  |
| ------- | ---------------------- |
| 0x00    | **0x21** (=<u>L</u>SB) |
| 0x01    | 0x43                   |
| 0x02    | 0x65                   |
| 0x03    | 0x87                   |

## Project

### Installation

#### VMware installation

##### Installing Rosetta

Needed in order to run x86_64 applications on Apple Silicon:

```bash
/usr/sbin/softwareupdate --install-rosetta --agree-to-license
```

##### Installing Vagrant

```bash
brew install vagrant
```

##### Installing VMWare Fusion 13

VMWare Fusion _Player_ is discontinued, use _Pro_ instead.

Click [here](https://support.broadcom.com/group/ecx/productdownloads?subfamily=VMware%20Fusion&freeDownloads=true) to download.

##### Installing VMWare Utility

There are two steps to install the VMWare Utility.

```bash
brew install --cask vagrant-vmware-utility
```

```bash
vagrant plugin install vagrant-vmware-desktop
```

##### Running Vagrant

Inside the project folder (!), run:

```bash
vagrant up
```

```bash
vagrant ssh
```
