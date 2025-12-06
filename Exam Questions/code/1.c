#include <pthread.h>
#include <stdio.h>

int A = 0;
pthread_t thread_id[1000];

void *count(void *input) {
  int i;
  for (i = 0; i < 1000; i++) {
    A++;
  }
  pthread_exit(NULL);
}

int main(void) {
  int i;
  for (i = 0; i < 1000; i++) {
    pthread_create(&thread_id[i], NULL, count, NULL);
  }
  for (i = 0; i < 1000; i++) {
    pthread_join(thread_id[i], NULL);
  }
  printf("%d\n", A);
  return 0;
}