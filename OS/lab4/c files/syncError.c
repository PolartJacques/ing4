
// SYNCHRONISATION ERROR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

void * increment (void * _i) {
  int * i = (int *) _i;
  int reg = *i;
  reg ++;
  *i = reg;
  pthread_exit(i);
}

void * decrement (void * _i) {
  int * i = (int *) _i;
  int reg = *i;

  // interupt thread
  sleep(2);
  
  reg --;
  *i = reg;
  pthread_exit(i);
}

void main() {
  int i = 65;

  // create threads
  pthread_t th1, th2;
  pthread_create(&th1, NULL, decrement, (void *) &i);
  sleep(1);
  pthread_create(&th2, NULL, increment, (void *) &i);
  
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  printf("%i\n", i);

  return;
}