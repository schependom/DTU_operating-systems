#include <stdio.h> // Many programs start with this. What is it??
#include <stdlib.h>
#include <string.h>
/************************/

void example0(void)
{
  int i;
  for (i=0;i<5;i++)
  {
    #include "code.c"
  }

}

/************************/

void example1(void)
{
  int i;

  for (i = 0; i<5; i++)
    printf("%d\n", i);

  for (i = 5; i>=0; i--)
    printf("%d\n", i);
}

/************************/

#define LOOPS 5

void example2(void)
{
  int i;

  for (i = 0; i<LOOPS; i++)
    printf("%d\n", i);

  for (i = LOOPS; i>=0; i--)
    printf("%d\n", i);
}

/************************/

#define ONE_PLUS_ONE 1+1

void example3(void)
{
  printf("%d\n", ONE_PLUS_ONE); // what will this print?

  printf("%d\n", ONE_PLUS_ONE + 10); // what will this print?

  printf("%d\n", ONE_PLUS_ONE * 10); // what will this print?

}

/************************/

#define ENABLE 1
int flag_enable = ENABLE;

void example4(void)
{
  if (flag_enable) // what will this print?
  {
    printf("The feature is enabled.\n");
  }
  else
  {
    printf("The feature is disabled.\n");
  }

// what will this print?
#if ENABLE
    printf("The feature is enabled.\n");
#else
    printf("The feature is disabled.\n");
#endif

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

  return 0;
}
