# DTU 02159 - Operating Systems

## Introduction to C-programming

I wrote out this little guide with things I learned while getting to know C programming. Hope it helps you too! -Vincent

### File structure

#### Including header files

To include a `.h` headerfile, use `#INCLUDE ...`.

##### Standard C libraries

- Use *angled* brackets: `<...>`
- Example:
```c
#include <stdio.h>
```
- `stdio`
  - 'Standard input-output'
  - `printf`, `scanf`, ...

##### Own libraries

- Use *double quotes*: `"..."`
- Example: 
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

### Running the code

Compiling to a binary executable (`.out` on Linux/Mac, `.exe` on Windows):

```bash
gcc -o outputfile sourcefile.c
```

Running the compiled binary:

```bash
./outputfile    -- on Linux/Mac
```

```bash
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

### IO

Make sure to include `<stdio.h>`.

#### Printing with `printf`

Formatting:
- `"%d"` or `"%i"` for `int`
- `"%c"` for `char`
- `"%f"` for `float`
- `"%lf"` for `double`
- `"%s"` for `string`
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