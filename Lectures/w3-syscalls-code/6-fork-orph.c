#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{

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

    printf("[%d] I am the child process and I sleep for 2 seconds.\n", getpid());
    sleep(2);

    printf("[%d] I am the child process and my parent is: %d\n", getpid(), getppid());

  }
  else {
    // parent process
    printf("[%d] I am the parent process and my parent is:%d\n", getpid(), getppid());

    printf("[%d] I am the parent process and I sleep for 1 seconds.\n", getpid());
    sleep(1);

    printf("[%d] I am the parent process and I terminate now.\n", getpid());
  }

  return 0;
}
