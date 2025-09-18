#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
A parent process MUST wait for its child processes to terminate.

    -> An ORPHAN PROCESS is a child process whose parents have terminated.

    -> A ZOMBIE PROCESS is a child process that has terminated,
        but whose PARENT has NOT WAITED for it to terminate,
        so it is STILL IN THE PROCESS TABLE

Waiting can be done with the wait() or waitpid() system calls.

    -> A parent can wait for ANY child process to terminate
          (wait() or waitpid(-1, ...))

    -> A parent can wait for a SPECIFIC child process to terminate
          (waitpid(child_pid, ...))

Waiting for the return code of the child is the SECOND WAY for a parent process
to get information from its child process.
    -> The first way is via shared memory with `mmap` (see 5-fork-shared.c)
*/

int main(void)
{
    int counter = 0;

    counter = 100;

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // fork a child process
    pid_t pid = fork();

    // check for errors
    if (pid == -1)
    {
        printf("fork fail\n");
        exit(1);
    }

    if (pid == 0)
    {
        // child process

        // code execution
        printf("c[%d] I am the child process and I execute some code.\n", getpid());
        char c = getchar(); // emulating a slow execution...

        int return_code = 58;

        printf("c[%d] I am the child process and I return: %d\n", getpid(), return_code);

        return return_code;
    }
    else
    {
        // parent process
        printf("p[%d] I am the parent and I will wait for the child %d to terminate.\n", getpid(), pid);

        // Integer that will store the exit code of the child process
        //      -> We will pass this address to waitpid()
        int child_status;

        // Wait for the specific child process to terminate using...
        //      -> PID of child
        //      -> pointer to an integer to store the exit code of the child
        //      -> options (0 means no special options)
        waitpid(pid, &child_status, 0);

        // WEXITSTATUS extracts the return code of the child process from the integer
        printf("c[%d] The child process returned: %d\n", getpid(), WEXITSTATUS(child_status));
    }

    return 0;
}
