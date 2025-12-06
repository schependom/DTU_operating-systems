#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int counter = 100;
    // -> the counter is copied in the fork
    // -> so we have two INDEPENDENT counters!

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // ! fork a child process
    // ! and store its PID in a variable
    pid_t pid = fork();
    // -> The _t in pid_t stands for TYPE
    // -> Common naming convention for data types defined using a typedef
    // -> This convention helps developers quickly identify that a variable is a custom type,
    //      rather than a basic built-in type like int or char.

    // now the running process (the one that created the child), is a PARENT PROCESS
    // the child is a CHILD PROCESS

    // The child process is an EXACT COPY of the parent process
    //      -> different address space
    //      -> SAME CONTENTS!

    // fork is the only way a process can be created
    //      -> so all processes are children of the init() process (PID 1)

    // always check if the systemcall worked (exit code 0 of syscall means success)
    if (pid == -1)
    {
        // error checking
        //      -> executed by both parent and child
        //      -> because the child is an exact copy of the parent
        printf("fork fail\n");
        exit(1);
        // exit with exit code 1
    }

    printf("[%d] Both parent and child will execute this.\n", getpid());

    // We make an identical copy, but the PID will be different!
    //      -> For the parent, the pid variable will contain the child's PID
    // !    -> For the child, the pid variable will be 0
    if (pid == 0)
    {
        // child process,
        // ! because the PID is 0 for the child!
        printf("[%d] (if) I am the child process and my parent is: %d\n", getpid(), getppid());
        sleep(1);

        int i;
        for (i = 0; i < 5; i++)
        {
            printf("[%d] child process: counter=%d\n", getpid(), ++counter);
        }
    }
    else
    {
        // parent process,
        // because the PID is the child's PID!
        printf("[%d] (else) I am the parent process and my parent is: %d\n", getpid(), getppid());
        sleep(1);

        int i;
        for (i = 0; i < 5; i++)
        {
            printf("[%d] parent process: counter=%d\n", getpid(), ++counter);
        }
    }

    // char c = getchar();

    // ! Everything that is not inside of the if-else, is exectuted by BOTH processes!
    return 0;
}
