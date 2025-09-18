#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  int counter = 100;

  printf("[%d] I am the parent process and I fork.\n", getpid());

  pid_t pid = fork(); // fork a child process
  if (pid == -1) { // error checking
    printf("fork fail\n");
    exit(1); // exit with exit code 1
  }

  printf("[%d] Both parent and child will execute this.\n", getpid());

  if (pid == 0) {
    // child process
    printf("[%d] I am the child process and my parent is: %d\n", getpid(), getppid());
    sleep(1);

    int i;
    for (i=0;i<5;i++) {
      printf("[%d] child process: counter=%d\n", getpid(), ++counter);
    }
  }
  else {
    // parent process
    printf("[%d] I am the parent process and my parent is: %d\n", getpid(), getppid());
    sleep(1);

    int i;
    for (i=0;i<5;i++) {
      printf("[%d] parent process: counter=%d\n", getpid(), ++counter);
    }
  }

  //char c = getchar();
  return 0;
}
