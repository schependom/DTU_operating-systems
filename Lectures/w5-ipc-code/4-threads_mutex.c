#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define AMOUNT 1
#define REPEAT 100000

// ! The mutex is a kernel-managed lock variable
// -> the kernel ensures that only one thread at a time can lock the mutex
// -> when released, the kernel chooses which waiting thread (if any) will get the mutex
pthread_mutex_t lock;

int balance = 0;
pthread_t thread_id[2];

void *withdraw(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
    {
        // Retrieve and lock the mutex
        pthread_mutex_lock(&lock);   // Lock
        balance = balance - AMOUNT;  // Access critical region
        pthread_mutex_unlock(&lock); // Unlock
        // -> the kernel ensures that only one thread at a time can lock the mutex
        // -> when released, the kernel chooses which waiting thread (if any) will get the mutex
    }
    pthread_exit(NULL); // Thread's job is done
}

void *deposit(void *input)
{
    int i;
    for (i = 0; i < REPEAT; i++)
    {
        pthread_mutex_lock(&lock);   // Lock
        balance = balance + AMOUNT;  // Access critical region
        pthread_mutex_unlock(&lock); // Unlock
    }
    pthread_exit(NULL); // Thread's job is done
}

int main(void)
{
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create two threads
    pthread_create(&thread_id[0], NULL, withdraw, NULL); // create a thread
    pthread_create(&thread_id[1], NULL, deposit, NULL);  // create a thread

    // Wait for both threads to finish
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    // Bug is fixed
    printf("Balance: %d \n", balance);
    return 0;
}
