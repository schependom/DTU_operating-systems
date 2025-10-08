#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define AMOUNT 10000

// WITH PERIFERALS -> scheduler switches from blocked process to another process

void withdraw(int amount)
{

    int balance;
    FILE *f = fopen("bank_account.txt", "r"); // Open Bank Account with read permission
    fscanf(f, "%d\n", &balance);              // Read Balance
    fclose(f);                                // Close Bank Account

    balance = balance - amount; // Withdraw Amount

    f = fopen("bank_account.txt", "w"); // Open Bank Account with write permission
    fprintf(f, "%d\n", balance);        // Write Balance
    fclose(f);                          // Close Account
    printf("Withdrawn %d\n", amount);
    exit(0);
}

void deposit(int amount)
{

    int balance;
    FILE *f = fopen("bank_account.txt", "r"); // Open Bank Account
    fscanf(f, "%d\n", &balance);              // Read Balance
    fclose(f);                                // Close Bank Account

    balance = balance + amount; // Deposit Amount

    f = fopen("bank_account.txt", "w"); // Open Bank Account
    fprintf(f, "%d\n", balance);        // Write Balance
    fclose(f);                          // Close Account
    printf("Deposited %d\n", amount);
    exit(0);
}

int main(void)
{
    int balance;
    pid_t pid1, pid2;
    FILE *f;

    /*
    ! Because we use fork, we don't have control over which child process
    ! executes first. So, the final balance may be either 10000 or -10000.
    */

    pid1 = fork(); // fork a child process
    if (pid1 == 0)
    {
        printf("Child process 1 (PID: %d) created and starting deposit.\n", getpid());
        deposit(AMOUNT); // Child deposits amount
        // at the end of deposit function,
        // child process exits
    }

    pid2 = fork(); // fork a child process
    if (pid2 == 0)
    {
        printf("Child process 2 (PID: %d) created and starting withdraw.\n", getpid());
        withdraw(AMOUNT); // Child withdraws amount
        // at the end of withdraw function,
        // child process exits
    }

    // waitpid takes three arguments:
    // 1. pid of the child process to wait for (-1 means wait for any child)
    // 2. pointer to an integer to store the exit status of the child (NULL means we don't care)
    // 3. options (0 means no special options)

    waitpid(-1, NULL, 0);
    // Wait for any child process (-1) to exit,
    // and we don't care about its exit status (NULL)
    // and there are no special options (0)
    waitpid(-1, NULL, 0);

    f = fopen("bank_account.txt", "r");
    fscanf(f, "%d\n", &balance);
    fclose(f);

    /*
    Scheduler chooses child process x first
        ->  we ask - via a syscall - to open a file
        ->  then we ask to read from it (slow!)
        ->  while this is blocked (waiting for the disk),
    !       the scheduler switches to child process y
            ->  we ask - via a syscall - to open a file
            ->  then we ask to read from it (slow!)
            ->  while this is blocked (waiting for the disk),
                the scheduler switches back to child process x
        ->  both processes read the same initial balance
        ->  the first process overwrites the balance written by the second process
        ->  final balance is 10000 (if x=first one was the deposit) or -10000 (if x=first one was the withdraw)
    */

    printf("Balance: %d\n", balance);

    return 0;
}
