#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char c;
void foo(int a, int *p);
void bar(void *p, char type);


/***************************************************/

void example0(void)
{

	int A = 5;

	printf("A is an integer\n");
	printf("The value of A: %d\n", A);
	printf("The memory address of A: %p\n\n", &A);

	scanf("%c",&c);

	int *B; // tip: int *B; is identical to int* B;

	B = &A;

	printf("B is a pointer\n");
	printf("The value of B: %p\n", B);
	printf("The memory address of B: %p\n", &B);
	printf("The value of the memory that B points at: %d\n\n", *B);

	printf("The size of a pointer to an integer is: %lu\n", sizeof(int*));
	printf("The size of a pointer to an character is: %lu\n", sizeof(char*));


}

/***************************************************/

void example1(void)
{

	int A,B;
	int *p;

	A = 5;
	B = A;
	B = B + 1;

	printf("What is the value of A?");
	scanf("%c",&c);

	printf("The value of A: %d\n\n", A);


	A = 5;
	p = &A;
	*p = *p + 1;

	printf("What is the value of A?");
	scanf("%c",&c);

	printf("The value of A: %d\n\n", A);

	scanf("%c",&c);

	A = 5;
	B = 5;

	printf("Outside of the foo function\n\n");

	printf("The value of A: %d\n", A);
	printf("The memory address of A: %p\n", &A);
	printf("The value of B: %d\n", B);
	printf("The memory address of B: %p\n\n", &B);

	p = &B;
	printf("The value of p: %p\n", p);
	printf("The memory address of p: %p\n", &p);
	printf("The value of the memory that p points at: %d\n\n", *p);

	scanf("%c",&c);
	foo(A,p);
	scanf("%c",&c);

	printf("After the foo function has returned\n\n");

	printf("The value of A: %d\n", A);
	printf("The memory address of A: %p\n", &A);
	printf("The value of B: %d\n", B);
	printf("The memory address of B: %p\n\n", &B);

	printf("The value of p: %p\n", p);
	printf("The memory address of p: %p\n", &p);
	printf("The value of the memory that p points at: %d\n\n", *p);

}

void foo(int A, int *p)
{
	printf("Inside the foo function\n\n");

	printf("The value of A: %d\n", A);
	printf("The memory address of A: %p\n\n", &A);

	printf("The value of p: %p\n", p);
	printf("The memory address of p: %p\n", &p);
	printf("The value of the memory p points at: %d\n", *p);

	scanf("%c",&c);

	A = A + 1;
	*p = *p + 1;

	printf("The value of A: %d\n", A);
	printf("The memory address of A: %p\n\n", &A);

	printf("The value of p: %p\n", p);
	printf("The memory address of p: %p\n", &p);
	printf("The value of the memory p points at: %d\n", *p);
}

/***************************************************/

void example2(void)
{
	char A = 'g';

	bar( (void*) &A, 'c' );


	int B = 1024;

	bar( (void*) &B, 'd' );


	float C = 3.14;

	bar( (void*) &C, 'f' );

}

/* This functions prints the contents of the memory
   pointed by p. The type is defined by t.  */
void bar(void *p, char type)
{
	if (type=='c') // character
		printf("%c\n", *(char*)p);
	else if (type=='d') // integer
		printf("%d\n", *(int*)p);
	else if (type=='f') // float
		printf("%f\n", *(float*)p);
	else
		printf("Unsupported type\n");

}

/***************************************************/

#define SIZE 8
void example3(void)
{
	int i;
	int A[SIZE];

	for (i=0;i<SIZE;i++)
		A[i] = i;

	for (i=0;i<SIZE;i++)
		printf("%p A[%d]: %d\n", &A[i], i, A[i]);

	scanf("%c",&c);

	printf("What will this print?");
	scanf("%c",&c);

	printf("%p\n\n", A);

	printf("What will this print?");
	scanf("%c",&c);

	printf("%p\n", A+4);

	scanf("%c",&c);

	*(A+4) = 100;

	for (i=0;i<SIZE;i++)
		printf("%p A[%d]: %d\n", &A[i], i, A[i]);


	scanf("%c",&c);

	char *C;

	C = (char*) A;

	printf("What will this print?");
	scanf("%c",&c);

	printf("%p\n\n", C+4);

}

/***************************************************/

#define SMALLSIZE 4
void example4(void)
{
	int i,j;

	int A[SMALLSIZE][SMALLSIZE];

	for (i=0;i<SMALLSIZE;i++)
		for (j=0;j<SMALLSIZE;j++)
			A[i][j] = i*SMALLSIZE + j;

	for (i=0;i<SMALLSIZE;i++)
		for (j=0;j<SMALLSIZE;j++)
			printf("%p A[%d][%d]: %d\n", &A[i][j], i, j, A[i][j]);

	printf("\nWhat will this print?");
	scanf("%c",&c);

	printf("%p\n\n", A);

	printf("What will this print?");
	scanf("%c",&c);

	printf("%p\n\n", *A);

	printf("What will this print?");
	scanf("%c",&c);

	printf("%d\n", **A);

	scanf("%c",&c);

	for (i=0;i<SMALLSIZE;i++)
		printf("%p\n", *(A+i));

	scanf("%c",&c);

	int *p;

	p = *(A+2);

	for (i=0;i<SMALLSIZE;i++)
		printf("%d\n", p[i]);

}

/***************************************************/
#define BLOCK 4096
void foo2(void)
{
	static int i=0;
  	char A[BLOCK];
  	printf("[%d] I am allocating %d bytes in the stack.\n", ++i, BLOCK);

}

void example5(void)
{
	while(1)
		foo2();
}

/***************************************************/

void example6(void)
{
  	static int i=0;
  	char A[BLOCK];

  	printf("[%d] I am allocating %d bytes in the stack.\n", ++i, BLOCK);

  	example6();
}

/***************************************************/

void foo3(void)
{
	static int i=0;

  	char *ptr = (char*)malloc(BLOCK);

  	printf("[%d] I am allocating %d bytes in the heap.\n", ++i, BLOCK);

}

void example7(void)
{
	while(1)
		foo3();
}

/***************************************************/

void foo4(void)
{
	static int i=0;


  	int *ptr = (int*)malloc(BLOCK);

  	printf("[%d] I am allocating %d bytes in the heap and I remember to free them!\n", ++i, BLOCK);

  	free(ptr);

}

void example8(void)
{
	while(1)
		foo4();
}

/***************************************************/

void bar2(int *p)
{
	p = (int*) malloc(4);
	*p = 15;
}

void example9(void)
{
	int *ptr = NULL;

	bar2(ptr);

	printf("What will this print?");
	scanf("%c",&c);

	printf("%d\n", *ptr);
}

/***************************************************/

void bar3(int *p)
{
	printf("Inside the function\n\n");

	printf("The address of the pointer is: %p\n", &p);
	printf("The value of the pointer is: %p\n\n", p);

	p = (int*) malloc(4);
	*p = 15;

	printf("The address of the pointer is: %p\n", &p);
	printf("The value of the pointer is: %p\n", p);
	printf("The value of the memory the pointer points at: %d\n\n", *p);
}

void example10(void)
{
	int *ptr = NULL;

	printf("Outside the function\n\n");

	printf("The address of the pointer is: %p\n", &ptr);
	printf("The value of the pointer is: %p\n\n", ptr);

	bar3(ptr);

	printf("Outside the function\n\n");

	printf("The address of the pointer is: %p\n", &ptr);
	printf("The value of the pointer is: %p\n", ptr);
	printf("The value of the memory the pointer points at: %d\n\n", *ptr);

	free(ptr);
}

/***************************************************/

void bar4(int **p)
{
	printf("Inside the function\n\n");

	printf("The address of the pointer is: %p\n", &p);
	printf("The value of the pointer is: %p\n", p);
	printf("The value of the second pointer is: %p\n\n", *p);

	*p = (int*) malloc(4);
	**p = 15;

	printf("The address of the pointer is: %p\n", &p);
	printf("The value of the pointer is: %p\n", p);
	printf("The value of the second pointer is: %p\n", *p);
	printf("The value of the memory the second pointer points at: %d\n\n", **p);
}

void example11(void)
{
	int *ptr = NULL;

	printf("Outside the function\n\n");

	printf("The address of the pointer is: %p\n", &ptr);
	printf("The value of the pointer is: %p\n\n", ptr);

	bar4(&ptr);

	printf("Outside the function\n\n");

	printf("The address of the pointer is: %p\n", &ptr);
	printf("The value of the pointer is: %p\n", ptr);
	printf("The value of the memory the pointer points at: %d\n\n", *ptr);

	free(ptr);
}

/***************************************************/

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
	else if (strcmp(argv[1],"6")==0)
    example6();
  else if (strcmp(argv[1],"7")==0)
    example7();
  else if (strcmp(argv[1],"8")==0)
    example8();
  else if (strcmp(argv[1],"9")==0)
    example9();
  else if (strcmp(argv[1],"10")==0)
    example10();
	else if (strcmp(argv[1],"11")==0)
	  example11();
  return 0;
}
