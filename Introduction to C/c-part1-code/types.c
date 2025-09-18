#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char c;
int i;

void example0(void)
{
  // short int is

  short int a = 65;
  // 65 is a 7-bit integer
  // 0100 0001 in binary

  printf("Example 0\n");

  /***************************************************/

  printf("What does the following statement print?\n");

  scanf("%c", &c);
  // address of c is passed to scanf
  // scanf stores the character read from stdin at that address

  printf("%d\n", a);
  // we pass the value of a (65) to printf
  // printf prints the value of a (65) as an integer (%d)

  /***************************************************/

  // store the input character at the address of c
  scanf("%c", &c);

  printf("What does the following statement print?\n");

  // store the input character at the address of c
  scanf("%c", &c);

  // print the value of a (65) as a character (%c)
  // ASCII code 65 is the character 'A'
  // so this prints 'A'
  printf("%c\n", a);

  /***************************************************/

  scanf("%c", &c);
  printf("What does the following statement print?\n");

  // store the input character at the address of c
  scanf("%c", &c);

  a = a + 2;

  // print the value of a (67 now) as a character (%c)
  printf("%c\n", a);

  /***************************************************/

  scanf("%c", &c);

  printf("%d\n", a);   // 67
  printf("%c\n", a);   // 67 as an ASCII character 'C'
  printf("0x%x\n", a); // 67 in hexadecimal -> 0x43
  printf("%o\n", a);   // 67 in octal -> 103

  // print all bits of a
  // start all the way to the right (bit 0) and go to the left (bit 15)
  // & is the bitwise AND operator
  // 0x1 is 1 in hexadecimal, which is 0000 0001 in binary
  for (i = 15; i >= 0; i--)
  {
    // 0x1 == 1 in binary: 0000 0001
    // Shift right i times
    printf("%d", (a >> i) & 0x1);
  }
  printf("\n");
}

void example1(void)
{
  // x is octal (because of leading 0)
  // 017_8 = 1*8^1 + 7*8^0 = 8 + 7 = 15_10
  int x = 017;
  // y is decimal
  int y = 16;

  printf("Example 1\n\n");

  /***************************************************/

  printf("x = 017 (octal) = %d (decimal)\n", x);
  printf("y = 16 (decimal) = %o (octal)\n\n", y);

  // is 15 > 16?
  printf("is x greater than y?");
  scanf("%c", &c);

  if (x > y)
    printf("yes\n");
  else
    printf("no\n"); // no, 15 is not greater than 16

  scanf("%c", &c);
  printf("x=%d y=%d\n", x, y);

  /***************************************************/

  // x is an int (>= 16 bits)
  // x = 0x10 = 16_10 = 0b0001 0000 in binary
  x = 0x10;
  // y is an int (>= 16 bits)
  // y = 0b0000 0001 = 1_10 = 0x01 in hexadecimal
  y = 0b0001;

  printf("x = 0x10 (hexadecimal) = %d (decimal)\n", x);
  printf("y = 0b0001 (binary) = %d (decimal)\n\n", y);

  printf("Is x==y?");
  scanf("%c", &c);
  if (x == y)
    printf("yes\n");
  else
    printf("no\n"); // no, 16 is not equal to 1
}

void example2(void)
{
  char A;      // >= 8 bits
  short int B; // >= 16 bits
  int C;       // >= short int
  long D;      // >= 32 bits

  printf("Example 2\n");

  /***************************************************/

  printf("what is the size of a character?");
  scanf("%c", &c);

  // lu = Long unsigned integer type.
  // -> because a size is always non-negative
  // -> sizeof returns a value of type size_t, which is an unsigned long
  printf("%lu\n", sizeof(A));
  // prints '1' because a char is 1 byte (8 bits)

  // SO: sizeof() returns the size in BYTES!

  /***************************************************/

  scanf("%c", &c);
  printf("what is the size of a short integer?");
  scanf("%c", &c);

  // sizeof() returns the size in BYTES of the type or variable passed to it

  // B is a short int, which is at least 2 bytes (16 bits)
  // On most systems, a short int is exactly 2 bytes (16 bits)
  printf("%lu\n", sizeof(B));
  // prints '2' (bytes)

  /***************************************************/

  scanf("%c", &c);
  printf("what is the size of an integer?");
  scanf("%c", &c);

  printf("%lu\n", sizeof(C));
  // Integer = 4 bytes (32 bits) on most systems

  /***************************************************/

  scanf("%c", &c);

  printf("what is the size of a long integer?");
  scanf("%c", &c);

  printf("%lu\n", sizeof(D));
  // Long = 8 bytes (64 bits) on this system

  /***************************************************/

  scanf("%c", &c);

  printf("what will the following print?\n");
  scanf("%c", &c);

  B = 70000;
  // 70 000 in decimal needs at least 17 bits in binary: 1 0001 0001 0111 0000
  // but (short int) B is only 16 bits

  // print the (overflowed) value of B as an unsigned integer (%u)
  // 0001 0001 0111 0000 = 4464_10
  printf("%u\n", B);
}

void example3(void)
{
  // char = 1 byte = 8 bits
  // call N = number of bits
  // -> N = 8

  // unsigned
  // -> [0, 2^N - 1]
  // -> [0, 2^8 - 1] = [0, 255] = [ASCII 0, ASCII 255]
  unsigned char A;
  // signed
  // -> [-2^(N-1), 2^(N-1) - 1]
  // -> [-2^7, 2^7 - 1] = [-128, 127] = [ASCII 0, ASCII 127]
  char B;

  // short int = 2 bytes = 16 bits
  // -> N = 16
  short unsigned int C;
  // unsigned -> [0, 2^N - 1] = [0, 2^16 - 1] = [0, 65535]

  // int = 4 bytes = 32 bits
  unsigned int D;
  // unsigned -> [0, 2^N - 1] = [0, 2^32 - 1] = [0, 4294967295]

  printf("Example 3\n");

  /***************************************************/

  // A was an unsigned char
  // A can store values in the range [0, 255]
  A = 255;

  printf("what will the following print?\n");
  scanf("%c", &c);

  printf("%d\n", A);

  /***************************************************/

  B = (char)A;

  printf("what will the following print?\n");

  scanf("%c", &c);

  printf("%d\n", B);

  /***************************************************/

  D = 70000;

  printf("what will the following print?\n");
  scanf("%c", &c);

  printf("%d\n", D);

  /***************************************************/

  C = (short unsigned int)D;

  printf("what will the following print?\n");
  scanf("%c", &c);

  printf("%d\n", C);
}

void example4(void)
{
  char K[4];

  printf("Example4\n");

  printf("what will the following print?\n");
  scanf("%c", &c);

  unsigned int k = 0x12345678;
  memcpy(K, &k, 4);

  printf("%.2x\n", K[0]);

  scanf("%c", &c);
  for (i = 0; i < 4; i++)
    printf("K[%d] = %.2x\n", i, K[i]);
}

int G = 10;

void example5(void)
{
  int G = 20;

  printf("Example 5\n");

  printf("what will the following print?\n");
  scanf("%c", &c);

  printf("%d\n", G);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("use: %s <example number>\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[1], "0") == 0)
    example0();
  else if (strcmp(argv[1], "1") == 0)
    example1();
  else if (strcmp(argv[1], "2") == 0)
    example2();
  else if (strcmp(argv[1], "3") == 0)
    example3();
  else if (strcmp(argv[1], "4") == 0)
    example4();
  else if (strcmp(argv[1], "5") == 0)
    example5();

  return 0;
}
