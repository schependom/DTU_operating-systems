#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
  int counter = 0;

  counter = 100;

  printf("[%d] I am the parent process and I fork.\n", getpid());

  pid_t pid = fork(); // fork a child process

  if (pid == -1) {
    printf("fork fail\n");
    exit(1);
  }

  if (pid == 0) {
    // child process

    // code execution
    printf("[%d] I am the child process and I execute some code.\n", getpid());
    char c = getchar(); // emulating a slow execution...

    int return_code = 58;

    printf("[%d] I am the child process and I return: %d\n", getpid(), return_code);

    return return_code;
  }
  else {
    // parent process
    printf("[%d] I am the parent and I will wait for the child %d to terminate.\n", getpid(), pid);

    int child_status;
    waitpid(pid, &child_status, 0); // waiting for the child to return

    printf("[%d] The child process returned: %d\n", getpid(), WEXITSTATUS(child_status) );

  }

  return 0;
}
