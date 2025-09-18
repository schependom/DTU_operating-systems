#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int *A;

int main(void)
{
  A = malloc(sizeof(int));

  *A = 50;

  printf("[%d] I am the parent process and I fork.\n", getpid());

  pid_t pid = fork(); // fork a child process
  if (pid == -1) { // error checking
    printf("fork fail\n");
    exit(1); // exit with exit code 1
  }

  printf("[%d] Both parent and child will execute this.\n", getpid());

  *A = *A + 1;
  printf("[%d] A = %d\n", getpid(), *A);

  return 0;
}
