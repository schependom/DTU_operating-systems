#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char c;
int i;

void example0(void)
{
  short int a = 65;

  printf("Example 0\n");

  /***************************************************/

  printf("What does the following statement print?\n");
  scanf("%c",&c);

  printf("%d\n", a);

  /***************************************************/

  scanf("%c",&c);
  printf("What does the following statement print?\n");
  scanf("%c",&c);

  printf("%c\n", a);

  /***************************************************/

  scanf("%c",&c);
  printf("What does the following statement print?\n");
  scanf("%c",&c);

  a = a + 2;

  printf("%c\n", a);

  /***************************************************/

  scanf("%c",&c);

  printf("%d\n", a);
  printf("%c\n", a);
  printf("%x\n", a);
  printf("%o\n", a);

  for (i=15;i>=0;i--)
  {
    printf("%d", (a>>i)&0x1);
  }
  printf("\n");

}

void example1(void)
{
  int x=017; int y=16;

  printf("Example 1\n\n");

  /***************************************************/

  printf("is x greater than y?");
  scanf("%c",&c);

  if (x > y)
    printf("yes\n");
  else
    printf("no\n");

  scanf("%c",&c);
  printf("x=%d y=%d\n", x, y);

  /***************************************************/

  x = 0x10;
  y = 0b0001;
}

void example2(void)
{
  char A;
  short int B;
  int C;
  long D;

  printf("Example 2\n");

  /***************************************************/

  printf("what is the size of a character?");
  scanf("%c",&c);

  printf("%lu\n",sizeof(A));

  /***************************************************/

  scanf("%c",&c);
  printf("what is the size of a short integer?");
  scanf("%c",&c);

  printf("%lu\n",sizeof(B));

  /***************************************************/

  scanf("%c",&c);
  printf("what is the size of an integer?");
  scanf("%c",&c);

  printf("%lu\n",sizeof(C));

  /***************************************************/

  scanf("%c",&c);

  printf("what is the size of a long integer?");
  scanf("%c",&c);

  printf("%lu\n",sizeof(D));

  /***************************************************/

  scanf("%c",&c);

  printf("what will the following print?\n");
  scanf("%c",&c);

  B = 70000;
  printf("%u\n", B);

}

void example3(void)
{
  unsigned char A;
  char B;

  short unsigned int C;
  unsigned int D;

  printf("Example 3\n");

  /***************************************************/

  A = 255;

  printf("what will the following print?\n");
  scanf("%c",&c);

  printf("%d\n", A);

  /***************************************************/

  B = (char) A;

  printf("what will the following print?\n");

  scanf("%c",&c);

  printf("%d\n", B);

  /***************************************************/

  D = 70000;

  printf("what will the following print?\n");
  scanf("%c",&c);

  printf("%d\n", D);

  /***************************************************/

  C = (short unsigned int) D;

  printf("what will the following print?\n");
  scanf("%c",&c);

  printf("%d\n", C);


}

void example4(void)
{
  char K[4];

  printf("Example4\n");

  printf("what will the following print?\n");
  scanf("%c",&c);

  unsigned int k = 0x12345678;
  memcpy(K, &k, 4);

  printf("%.2x\n", K[0]);

  scanf("%c",&c);
  for(i=0;i<4;i++)
    printf("K[%d] = %.2x\n", i, K[i]);
}

int G = 10;

void example5(void)
{
  int G = 20;

  printf("Example 5\n");

  printf("what will the following print?\n");
  scanf("%c",&c);

  printf("%d\n", G);

}

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("use: %s <example number>\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[1],"0")==0)
    example0();
  else if (strcmp(argv[1],"1")==0)
    example1();
  else if (strcmp(argv[1],"2")==0)
    example2();
  else if (strcmp(argv[1],"3")==0)
    example3();
  else if (strcmp(argv[1],"4")==0)
    example4();
  else if (strcmp(argv[1],"5")==0)
    example5();

  return 0;
}
