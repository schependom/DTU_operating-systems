#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
A pipe is a UNIDIRECTIONAL communication channel
    -> Data written to the WRITE end of the pipe can be read from the READ end of the pipe
    -> Pipes are used for inter-process communication (IPC)
    -> Pipes are often used to connect the output of one process to the input of another process

This is the THIRD WAY for a parent process to get information from its child process.
    -> The first way is via SHARED MEMORY with `mmap` (see 5-fork-shared.c)
    -> The second way is via the RETURN CODE of the child process (see 7-wait.c)
*/

int main()
{
    //* Store the read and write
    //* file descriptors of the pipe
    //* in a 2-element array
    int link[2];

    // Process ID
    pid_t pid;

    // Buffer to store data read from the pipe
    // *-> 4096 bytes is the typical size of a pipe buffer in Linux
    char buf[4096];

    // Create a pipe by calling the pipe() system call
    //  and passing the array with the read+write file descriptors
    if (pipe(link) == -1)
    {
        printf("pipe fail\n");
        exit(1);
    }

    // * Read-end file descriptor is link[0]
    // * Write-end file descriptor is link[1]
    printf("link[0]: %d and link[1]: %d\n", link[0], link[1]);

    // Fork a child process and store the PID
    //  of the new process in pid
    if ((pid = fork()) == -1)
    {
        // Error handling
        printf("fork fail\n");
        exit(1);
    }

    if (pid == 0)
    {

        // CHILD PROCESS (because pid == 0)

        // Close the read end of the pipe in the child
        //  because the child will only write to the pipe
        close(link[0]);

        // Construct to message in a char array = string
        char msg[] = "hello\n";

        // Write the message to the write end of the pipe
        write(link[1], msg, sizeof(msg));
        // We pass
        //      -> the FD of the write end of the pipe
        //      -> a pointer to the message
        //      -> the size of the message in bytes

        // Because msg[] is an array of which we know the size at compile time,
        //  we can use sizeof(msg) to get the size in bytes.
    }
    else
    {
        // PARENT PROCESS (because pid > 0)

        // Close the write end of the pipe in the parent
        //  because the parent will only read from the pipe
        close(link[1]);

        // Read from the read end of the pipe
        //  and continue until you have read sizeof(buf) bytes)
        int nbytes = read(link[0], buf, sizeof(buf));

        //* Read stops reading when
        //*     -> it has read sizeof(buf) bytes
        //*     -> there is no more data to read (the write end of the pipe is closed)
        //*     -> an error occurs

        // Print,
        //  starting from pointer (address) buf,
        //  a string of length nbytes
        //  to standard output FD (1)

        // %.*s prints a string with a specified length
        //      -> * is replaced by the length (nbytes)
        //      -> s is replaced by the string (buf)
        printf("I am the parent and I got the message: %.*s", nbytes, buf);

        // Wait for the child process to terminate
        // !    -> -1 means wait for ANY child process
        //      -> NULL means we don't care about the exit status of the child (we give no address to store it)
        //      -> 0 means no special options
        waitpid(-1, NULL, 0);
    }
    return 0;
}
