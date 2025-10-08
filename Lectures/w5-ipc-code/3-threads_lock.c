#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define AMOUNT 1
#define REPEAT 100000

#define UNLOCKED 0
#define LOCKED 1
int lock = UNLOCKED;

int balance = 0;
pthread_t thread_id[2];

void *withdraw(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
    {
        // Wait until lock is UNLOCKED
        while (lock == LOCKED)
            ;
        // ! ---- process can be stopped here by the scheduler ----
        lock = LOCKED;              // ! Lock variable is vulnerable to race condition !
        balance = balance - AMOUNT; // Access critical region
        lock = UNLOCKED;            // Unlock
    }
    pthread_exit(NULL); // Thread's job is done
}

void *deposit(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
    {
        // Idem
        while (lock == LOCKED)
            ; // Block until is unlocked...
        // ! ---- process can be stopped here by the scheduler ----
        lock = LOCKED;              // ! Lock = vulnerable to race condition !
        balance = balance + AMOUNT; // Access critical region
        lock = UNLOCKED;            // Unlock
    }
    pthread_exit(NULL); // Thread's job is done
}

int main(void)
{
    pthread_create(&thread_id[0], NULL, withdraw, NULL); // create a thread
    pthread_create(&thread_id[1], NULL, deposit, NULL);  // create a thread

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    // We still haven't fixed the bug
    printf("Balance: %d \n", balance);
    return 0;
}
