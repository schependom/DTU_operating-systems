#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int *A;
// GLOBAL pointer variable
// to store the address of an integer

int main(void)
{

    // allocate 4 bytes in the heap
    // (of each process, so two different address spaces!!)
    // FOR A (because malloc returns an address)
    A = malloc(sizeof(int));

    // set the value at the address stored in A to 50
    // (dereference the pointer A)
    *A = 50;

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // fork a child process
    pid_t pid = fork();

    // always check if the systemcall worked (exit code 0 of syscall means success)
    if (pid == -1)
    {
        printf("fork fail\n");
        exit(1); // exit with exit code 1
    }

    printf("[%d] Both parent and child will execute this.\n", getpid());

    // Both processes increment the value at the address stored in A
    *A = *A + 1;
    // -> because the address space is different,
    //    the value at the address stored in A is different for both processes!!
    // -> Both processes print 51, NOT 52!

    // Print the value at the address stored in A
    printf("[%d] A = %d\n", getpid(), *A);

    return 0;
}
