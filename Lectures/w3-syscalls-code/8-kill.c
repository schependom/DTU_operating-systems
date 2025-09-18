#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig) {
  if (sig == SIGINT) {
    printf("\nLooks like you hit Ctrl-C. I ignore you!\n");
  }
  else if (sig == SIGTERM) {
    printf("Looks like you sent me a termination signal. I still ignore you!\n");
  }
}

int main(void) {

  signal(SIGINT, handler); // setting up the same handler for two signals
  signal(SIGTERM, handler);

  while(1){
    printf("[%d] Loop\n", getpid());
    sleep(1);
  }

  return 0;
}
