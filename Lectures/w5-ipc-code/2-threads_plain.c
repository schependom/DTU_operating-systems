#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define AMOUNT 1
#define REPEAT 100000

// WITH THREADS -> scheduler switches from one thread to another thread
// even more unpredictable

int balance = 0;
pthread_t thread_id[2]; // pthread holds thread identifiers

void *withdraw(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
        balance = balance - AMOUNT;
    pthread_exit(NULL); // Thread's job is done
}

void *deposit(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
        balance = balance + AMOUNT;
    pthread_exit(NULL); // Thread's job is done
}

int main(void)
{
    pthread_create(&thread_id[0], NULL, withdraw, NULL); // create a thread
    pthread_create(&thread_id[1], NULL, deposit, NULL);  // create a thread

    // pthread_join waits for a specific thread to exit
    // -> arg 1: thread identifier
    // -> arg 2: return value of the thread (not used here)
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    printf("Balance: %d \n", balance);

    /*
    ! The final balance may be anything between -100000 and 100000
            because the two threads may switch at any time.
            For instance, the withdraw thread may execute 50000 times,
            then the deposit thread may execute 100000 times,
            and finally the withdraw thread may execute the remaining 50000 times.

    ! This is an example of a RACE CONDITION
    ! -> the final result depends on the timing of process/thread execution (scheduling)
    */

    return 0;
}
