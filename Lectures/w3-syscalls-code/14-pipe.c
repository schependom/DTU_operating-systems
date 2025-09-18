#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
A pipe is a UNIDIRECTIONAL communication channel
    -> Data written to the write end of the pipe can be read from the read end of the pipe
    -> Pipes are used for inter-process communication (IPC)
    -> Pipes are often used to connect the output of one process to the input of another process

This is the THIRD WAY for a parent process to get information from its child process.
    -> The first way is via SHARED MEMORY with `mmap` (see 5-fork-shared.c)
    -> The second way is via the RETURN CODE of the child process (see 7-wait.c)
*/

int main()
{
  int link[2];
  pid_t pid;
  char buf[4096];

  if (pipe(link) == -1)
  { // creates a pipe
    printf("pipe fail\n");
    exit(1);
  }

  printf("link[0]: %d and link[1]: %d\n", link[0], link[1]);

  if ((pid = fork()) == -1)
  { // forks a child process
    printf("fork fail\n");
    exit(1);
  }

  if (pid == 0)
  { // I am the child process

    close(link[0]);
    char msg[] = "hello\n";
    write(link[1], msg, sizeof(msg)); // write to pipe
  }
  else
  {

    close(link[1]);
    int nbytes = read(link[0], buf, sizeof(buf)); // read from pipe
    printf("I am the parent and I got the message: %.*s", nbytes, buf);

    waitpid(-1, NULL, 0); // wait for child to terminate
  }
  return 0;
}
