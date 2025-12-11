#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int a = 0;
    fork();
    printf("%d\n", a); // parent and child print 0 (2)
    fork();            // child_p and child_c created (4)
    ++a;
    printf("%d\n", a);    // parent: 1, child: 1, child_p: 1, child_c: 1
    fork();               // four new processes created (8)
    waitpid(-1, NULL, 0); // wait for any child process to terminate
    exit(0);
}

/*
Some OS's only create copies on WRITE operations (Copy-On-Write).

If we assume no copy-on-write mechanism is used, the number of distinct copies of the parent process's memory
that are created during the execution of this program is 7.
*/