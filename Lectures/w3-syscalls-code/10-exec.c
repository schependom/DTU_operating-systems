#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
! execl REPLACES THE CORE IMAGE of the current process

! The ONLY way we can create processes is with fork().

! New programmes can be executed in two steps:
    (1) The parent process calls fork() to create a clone child process.
    (2) The child process calls execl() to replace its code with another executable
*/

int main(void)
{
    int counter = 0;

    counter = 100;

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // fork a child process
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("fork fail\n");
        exit(1);
    }

    if (pid == 0)
    {
        // child process

        printf("[%d] I am the child process and call 'ls -1' with execl().\n", getpid());

        // Execute the "ls -1" command
        // execl(const char *path, const char *arg, ..., (char *)0);
        // The (char *)0 marks the end of the arguments
        execl("/bin/ls", "ls", "-1", (char *)0);

        // if execl doesn't fail, the child process image is replaced by the "ls" command
        // ! so the following lines will not be executed!

        // if execl fails, the child process will continue executing this code

        // this should not get executed
        printf("execl fail");
        return 1;
    }
    else
    {
        // parent process
        printf("[%d] I am the parent and I will wait for the child %d to terminate.\n", getpid(), pid);

        // Wait for the child process to terminate
        // and store the return code
        int child_status;
        waitpid(pid, &child_status, 0);

        // WEXITSTATUS extracts the return code from the status value
        printf("[%d] The child process returned: %d\n", getpid(), WEXITSTATUS(child_status));

        // If execl was successful, the return code should be 0, and
        //!  this return code originates from the "ls" command!

        // If execl failed, the return code should be 1, and
        //!  this return code originates from the child process itself!
    }
}
