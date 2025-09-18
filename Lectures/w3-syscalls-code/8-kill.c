#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
We can't kill this specific loop with
    ->  `kill [pid]` in terminal
        -> sends the SIGTERM signal to the process
    -> CTRL+C
        -> sends the SIGINT signal to the process
because of the signal handler we set up.

But we CAN kill the process with
    -> `kill -9 [pid]` in terminal
        -> sends the SIGKILL signal to the process
!       -> SIGKILL cannot be caught or ignored by the process
!       -> the process is terminated immediately by the kernel
*/

/*
*❯ kill -l
HUP INT QUIT ILL TRAP ABRT EMT FPE KILL BUS SEGV SYS PIPE ALRM TERM URG STOP TSTP CONT CHLD TTIN TTOU IO XCPU XFSZ VTALRM PROF WINCH INFO USR1 USR2

    -> The above are all the signals defined in POSIX
            -> Closing the terminal sends a SIGHUP signal
            -> CTRL+C sends a SIGINT signal
            -> `kill [pid]` in terminal sends a SIGTERM signal
            -> `kill -9 [pid]` in terminal sends a SIGKILL signal

    -> Some signals can be caught or ignored by the process (like SIGINT and SIGTERM)
    -> Some signals cannot be caught or ignored by the process (like SIGKILL and SIGSTOP)

    -> For some signals, the behavior can be changed by the process (like SIGINT and SIGTERM)
    -> For some signals, the behavior cannot be changed by the process (like SIGKILL and SIGSTOP)
*/

void handler(int sig)
{
    if (sig == SIGINT) // SIGINT = 2
    {
        // CTRL+C

        printf("\nLooks like you hit Ctrl-C. I ignore you!\n");

        // * SIGINT
        // *    Normally, we would do some cleaning up here, like freeing memory, closing files, etc.
        // *    before actually terminating the process.

        // But here, we just ignore the signal.
    }
    else if (sig == SIGTERM) // SIGTERM = 15
    {
        // kill [pid] in terminal
        printf("Looks like you sent me a termination signal. I still ignore you!\n");
    }

    // ! SIGKILL cannot be caught or ignored !
    //      -> sent by `kill -9 [pid]` in terminal
}

int main(void)
{

    /*
     * Signals are software-based interrupts
     *    -> interrupt normal execution to run a handler function
     *    -> default signal handler is executed, unless a custom handler is set up
     *        like we do here with the signal() function
     */

    // setting up the same handler for two signals
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    /*
    signal(int signum, void (*handler)(int))

          -> signum is the signal number (e.g., SIGINT, SIGTERM, etc. are predefined signal numbers (macros))

          -> handler is a pointer to a function that takes an int as an argument
              and returns void

          -> when the process receives the signal signum,
              the function handler is called with signum as its argument
    */

    while (1)
    {
        printf("[%d] Loop\n", getpid());
        sleep(1);
    }

    return 0;
}
