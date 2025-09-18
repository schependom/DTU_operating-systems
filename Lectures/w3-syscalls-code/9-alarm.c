#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int flag = 1;

void alarm_handler(int sig) {
  if (sig == SIGALRM) {
    flag = 0;
  }
}

int main(void) {
  int counter = 0;

  signal(SIGALRM, alarm_handler); // setting up a handler for the alarm signal

  printf("I am setting an alarm signal in 5 seconds\n");
  alarm(5);

  while(flag == 1) {
	   printf("Loop\n");
	   sleep(1);
  }

  printf("Done.\n");

  return 0;
}
