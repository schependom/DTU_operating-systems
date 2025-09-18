#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int flag = 1;

/*
An ALARM schedules a SIGALRM signal to be sent to the process
after a specified number of seconds.
*/

void alarm_handler(int sig)
{
    // When the process receives a SIGALRM signal,
    // this handler function is executed.
    if (sig == SIGALRM)
    {
        flag = 0;
    }
}

int main(void)
{
    int counter = 0;

    // * signal() registers a signal handler for a specific signal
    // It takes two arguments:
    //  -> signum (int)
    //  -> handler (sighandler_t is the typedef for a pointer to a function that takes an int and returns void)
    signal(SIGALRM, alarm_handler);

    printf("I am setting an alarm signal in 5 seconds\n");

    // Wait 5 seconds and send a SIGALRM signal to THIS process
    //  (so the process sends the signal to itself!)
    alarm(5);

    while (flag == 1)
    {
        printf("Loop\n");
        sleep(1);
    }

    // When the alarm goes off, the signal handler sets flag to 0,
    // so we exit the loop.

    printf("Done.\n");

    return 0;
}
