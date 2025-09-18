#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int link[2];
  pid_t pid;
  char buf[4096];

  if (pipe(link)==-1) { // creates a pipe
    printf("pipe fail\n");
    exit(1);
  }

  if ((pid = fork()) == -1) { // forks a child process
    printf("fork fail\n");
    exit(1);
  }

  if(pid == 0) { // I am the child process

    dup2(link[1], STDOUT_FILENO); // copy the input of the pipe to STDOUT

    close(link[0]);
    close(link[1]);

    execl("/bin/ls", "ls", "-1", (char *)0); // execute ls

    // this should not get executed
    printf("execl fail");
    exit(1);

  } else {

    close(link[1]);
    int nbytes = read(link[0], buf, sizeof(buf)); // read from pipe
    printf("%.*s", nbytes, buf);

    waitpid(-1, NULL, 0); // wait for child to terminate

  }
  return 0;
}
