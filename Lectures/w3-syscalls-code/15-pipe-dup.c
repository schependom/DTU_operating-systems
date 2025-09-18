#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{

    // Create an array to hold the read and write file descriptors of the pipe
    int link[2];

    // Process ID
    pid_t pid;
    // Buffer to hold data read from the pipe
    char buf[4096];

    // Create a pipe.
    //      -> link[0] is the read end of the pipe
    //      -> link[1] is the write end of the pipe
    if (pipe(link) == -1)
    {
        printf("pipe fail\n");
        exit(1);
    }

    // Fork a child process
    if ((pid = fork()) == -1)
    {
        printf("fork fail\n");
        exit(1);
    }

    if (pid == 0)
    {

        // I am the child process

        // Copy the input of the pipe to STDOUT (using the FD no of STDOUT)
        //      -> link[1] is the fd for the write end
        //      -> STDOUT_FILENO is the fd for standard output (1)
        dup2(link[1], STDOUT_FILENO);

        // Close the read and write ends of the pipe in the child
        close(link[0]);
        close(link[1]);

        // Execute the "ls -1" command
        //      -> execl REPLACES THE CORE IMAGE of the current process
        // *    -> The standard out of the "ls -1" command is now the write end of the pipe!
        execl("/bin/ls", "ls", "-1", (char *)0);

        // This should not get executed, unless execl fails
        printf("execl fail");
        exit(1);
    }
    else
    {

        // I am the parent process

        // Close the write end of the pipe
        close(link[1]);

        // read from the read end of the pipe
        int nbytes = read(link[0], buf, sizeof(buf));

        // %.*s prints a string with a specified length
        // here, we print nbytes characters from buf
        printf("%.*s", nbytes, buf);

        // wait for child to terminate
        //      -> NULL means we don't care about the exit status of the child (we give no address to store it)
        //      -> 0 means no special options
        waitpid(-1, NULL, 0);
    }
    return 0;
}
