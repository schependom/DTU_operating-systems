#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{

    int i = 0;

    while (1)
    {
        // different parent process id (PPID) if we run it
        // in two split terminals
        printf("[%d] My PID is %d and my parent's PID is %d\n", i++, getpid(), getppid());
        sleep(1);

        // if we kill and restart one of the processes,
        // the PPID will stay the same (because we're in the same terminal)
        // but the PID will change (because we started a new process)
    }

    return 0;
}
