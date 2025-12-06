#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int *A;

/*
Because both processes (parent and child) have their own separate memory space,
modifications made to the variable A in one process do not affect the other process.

Thus, when the parent process increments A to 51, the child process still sees A as 50,
and when the child process increments A to 51, the parent process still sees A as 51.
*/

int main(void)
{
  A = malloc(sizeof(int));

  *A = 50;

  printf("[%d] I fork.\n", getpid());

  pid_t pid = fork(); // fork a child process
  if (pid == -1)
  { // error checking
    printf("fork fail\n");
    exit(1); // exit with exit code 1
  }

  *A = *A + 1;
  printf("[%d] A = %d\n", getpid(), *A);

  return 0;
}
