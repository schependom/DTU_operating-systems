#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int *A;

int main(void)
{
  A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

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
