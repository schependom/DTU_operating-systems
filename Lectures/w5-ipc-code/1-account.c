#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define AMOUNT 10000

void withdraw(int amount){

  int balance;
  FILE *f = fopen("bank_account.txt","r"); // Open Bank Account
  fscanf(f, "%d\n", &balance);             // Read Balance
  fclose(f);                               // Close Bank Account

  balance = balance - amount;              // Withdraw Amount

  f = fopen("bank_account.txt","w");       // Open Bank Account
  fprintf(f, "%d\n", balance);             // Write Balance
  fclose(f);                               // Close Account
  exit(0);
}

void deposit(int amount){

  int balance;
  FILE *f = fopen("bank_account.txt","r"); // Open Bank Account
  fscanf(f, "%d\n", &balance);             // Read Balance
  fclose(f);                               // Close Bank Account

  balance = balance + amount;              // Deposit Amount

  f = fopen("bank_account.txt","w");       // Open Bank Account
  fprintf(f, "%d\n", balance);             // Write Balance
  fclose(f);                               // Close Account
  exit(0);
}

int main(void){
  int balance;
  pid_t pid1, pid2;
  FILE *f;

  pid1 = fork(); // fork a child process
  if (pid1 == 0) {
    deposit(AMOUNT); // Child deposits amount
  }

  pid2 = fork(); // fork a child process
  if (pid2 == 0) {
    withdraw(AMOUNT); // Child withdraws amount
  }

  waitpid(-1,NULL,0);
  waitpid(-1,NULL,0);

  f = fopen("bank_account.txt","r");
  fscanf(f, "%d\n", &balance);
  fclose(f);

  printf("Balance: %d\n", balance);

  return 0;
}
