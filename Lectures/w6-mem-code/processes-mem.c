#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int A;

int main(void)
{
  A = 50;

  printf("[%d] I fork.\n", getpid());

  pid_t pid = fork(); // fork a child process
  if (pid == -1)
  { // error checking
    printf("fork fail\n");
    exit(1); // exit with exit code 1
  }

  A = A + 1;
  printf("[%d] A = %d, address: %p\n", getpid(), A, &A);
  // this prints the
  // ! VIRTUAL ADDRESS,   which is the
  // ! SAME               in parent and child processes

  return 0;

  /*
  Parent and child processes have their own copy of the variable A.
  They each think they own it exclusively, but
  ! each process has its own physical address space,
  ! even though the virtual addresses are the same.
  */
}
