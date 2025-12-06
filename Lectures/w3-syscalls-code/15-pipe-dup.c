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

        // dup2(oldfd, newfd);
        //      -> First argument is the old fd
        //      -> Second argument is the new fd
        //
        //      -> Closes newfd if it is open, and makes newfd be the copy of oldfd
        //      -> After this call, both file descriptors refer to the same open file description
        //
        // Example:
        //      saved_stdin = dup(0); // save the current file in fd 0 (stdin) in the highest free fd (e.g. 5)
        //      dup2(saved_stdin, 0); // restore stdin from saved-stdin by copying the contents from fd slot 5 to fd slot 1

        // Redirect everything that comes in at STDOUT (fd 1)
        //  to the write end of the pipe (link[1])
        //
        //      -> In this way, anything the child process writes to STDOUT
        //         will go to the pipe instead of the terminal
        dup2(link[1], STDOUT_FILENO);

        /*
            -> After this call, any data that the program writes to standard output using functions like printf, puts,
                or write(STDOUT_FILENO, ...) will no longer go to the terminal.

            -> Instead, it will be written to the write end of the pipe,
                where it can be read from the pipe's read end (link[0]) by another process.

        This is a fundamental technique for inter-process communication (IPC), commonly
        used in shell commands like ls | grep. The output of the ls command
        is redirected to the input of the grep command via a pipe.
        */

        // Close the read and write ends of the pipe in the child
        //  (fd's not needed anymore, because we duplicated link[1] to STDOUT's fd slot 1)
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
        // because the parent will only read from the pipe
        close(link[1]);

        // read from the read end of the pipe
        int nbytes = read(link[0], buf, sizeof(buf));

        // Close the read end of the pipe
        close(link[0]);

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
