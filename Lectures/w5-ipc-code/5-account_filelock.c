#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define AMOUNT 10000

FILE *lock;

void withdraw(int amount)
{

  int balance;
  // Lock file
  //  with F_LOCK
  // -> OS ensures that only one process at a time can open the file
  // ! If this fails, the process is blocked until the file is unlocked
  lockf(fileno(lock), F_LOCK, 0);           // Lock
  FILE *f = fopen("bank_account.txt", "r"); // Open Bank Account
  fscanf(f, "%d\n", &balance);              // Read Balance
  fclose(f);                                // Close Bank Account

  balance = balance - amount; // Withdraw Amount

  f = fopen("bank_account.txt", "w"); // Open Bank Account
  fprintf(f, "%d\n", balance);        // Write Balance
  fclose(f);                          // Close Account

  // Release lock
  //  with F_ULOCK
  lockf(fileno(lock), F_ULOCK, 0); // Unlock
  exit(0);
}

void deposit(int amount)
{

  int balance;
  lockf(fileno(lock), F_LOCK, 0); // Lock
  // ! If this fails, the process is blocked until the file is unlocked
  FILE *f = fopen("bank_account.txt", "r"); // Open Bank Account
  fscanf(f, "%d\n", &balance);              // Read Balance
  fclose(f);                                // Close Bank Account

  balance = balance + amount; // Deposit Amount

  f = fopen("bank_account.txt", "w"); // Open Bank Account
  fprintf(f, "%d\n", balance);        // Write Balance
  fclose(f);                          // Close Account
  lockf(fileno(lock), F_ULOCK, 0);    // Unlock
  exit(0);
}

int main(void)
{
  int balance;
  pid_t pid1, pid2;
  FILE *f;

  lock = fopen("lock", "w+");

  pid1 = fork(); // fork a child process
  if (pid1 == 0)
  {
    deposit(AMOUNT); // Child deposits amount
  }

  pid2 = fork(); // fork a child process
  if (pid2 == 0)
  {
    withdraw(AMOUNT); // Child withdraws amount
  }

  waitpid(-1, NULL, 0);
  waitpid(-1, NULL, 0);

  f = fopen("bank_account.txt", "r");
  fscanf(f, "%d\n", &balance);
  fclose(f);

  printf("Balance: %d\n", balance);

  return 0;
}
