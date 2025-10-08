#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *A;

void *thread(void *input)
{
  *A = *A + 1;
  printf("[%d] A = %d\n", getpid(), *A);
  pthread_exit(NULL);
}

int main(void)
{
  pthread_t tid;

  A = malloc(sizeof(int));

  *A = 50;

  printf("[%d] I create a thread.\n", getpid());
  pthread_create(&tid, NULL, thread, NULL);

  *A = *A + 1;
  printf("[%d] A = %d\n", getpid(), *A);

  pthread_join(tid, NULL); // Wait for thread to exit

  return 0;
}
