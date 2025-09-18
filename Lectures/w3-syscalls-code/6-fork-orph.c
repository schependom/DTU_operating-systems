#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
A parent process MUST wait for its child processes to terminate.

    -> An ORPHAN PROCESS is a child process whose parents have terminated.

    -> A ZOMBIE PROCESS is a child process that has terminated,
        but whose PARENT has NOT WAITED for it to terminate,
        so it is STILL IN THE PROCESS TABLE
*/

int main(void)
{

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // fork a child process
    pid_t pid = fork();

    // error checking
    if (pid == -1)
    {
        printf("fork fail\n");
        exit(1); // exit with exit code 1
    }

    printf("[%d] Both parent and child will execute this.\n", getpid());

    // child process
    if (pid == 0)
    {
        printf("c[%d] I am the child process and my parent is: %d\n", getpid(), getppid());

        printf("c[%d] I am the child process and I sleep for 2 seconds.\n", getpid());
        sleep(2);

        printf("c[%d] I am the child process and my parent is: %d\n", getpid(), getppid());
    }
    else
    {
        // parent process

        printf("p[%d] I am the parent process and my parent is:%d\n", getpid(), getppid());

        printf("p[%d] I am the parent process and I sleep for 1 seconds.\n", getpid());
        sleep(1);

        printf("p[%d] I am the parent process and I terminate now.\n", getpid());
    }

    return 0;

    // Parent process terminates first, before it has to wait 1 second less.
}
