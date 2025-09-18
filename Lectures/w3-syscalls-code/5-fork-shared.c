#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int *A;
// again, a global pointer variable

/*
The FIRST WAY for parent and child to communicate, is with shared memory.
The SECOND WAY is with waitpid to check the return code (see 7-wait.c).
*/

int main(void)
{
    // mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // -> creates a memory mapping from the process's address space to a shared memory region
    // -> the memory region can be shared between related processes (like parent and child after a fork)
    // -> returns a pointer to the mapped area
    // -> NULL lets the kernel choose the address at which to create

    /*  MMAP

        -> specified as void *mmap
        -> means the function returns a GENERIC POINTER of type void *
        -> this pointer points to the start of the memory region

        -> we can cast this pointer to any type of pointer we want
           (here, we cast it to an int * pointer, because we want to store an integer there)
    */

    *A = 50;

    printf("[%d] I am the parent process and I fork.\n", getpid());

    // fork a child process
    pid_t pid = fork();

    if (pid == -1)
    { // error checking
        printf("fork fail\n");
        exit(1); // exit with exit code 1
    }

    printf("[%d] Both parent and child will execute this.\n", getpid());

    // Both child and parent increment the value at the address stored in A
    *A = *A + 1;

    // Because A is now stores in a SHARED memory region,
    // both processes will see the incremented value!
    printf("[%d] A = %d\n", getpid(), *A);
    // Now, we DO print
    // A = 51 ... A = 52
    // (<> 4-fork-global.c!)

    return 0;
}
