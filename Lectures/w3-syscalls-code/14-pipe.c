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

  printf("link[0]: %d and link[1]: %d\n", link[0], link[1]);

  if ((pid = fork()) == -1) { // forks a child process
    printf("fork fail\n");
    exit(1);
  }

  if(pid == 0) { // I am the child process

  close(link[0]);
  char msg[] = "hello\n";
  write(link[1], msg, sizeof(msg)); // write to pipe

  } else {

    close(link[1]);
    int nbytes = read(link[0], buf, sizeof(buf)); // read from pipe
    printf("I am the parent and I got the message: %.*s", nbytes, buf);

    waitpid(-1, NULL, 0); // wait for child to terminate

  }
  return 0;
}
