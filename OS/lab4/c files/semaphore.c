
// RESOLVE SYNCHRONISATION ERROR WITH SEMAPHORE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

struct arg {
  sem_t * sem;
  int * i;
};

void * increment (void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  int * i = arg->i;
  sem_t * sem = arg->sem;

  sem_wait(sem);

  int reg = *i;
  reg ++;
  *i = reg;

  sem_post(sem);
  pthread_exit(i);
}

void * decrement (void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  int * i = arg->i;
  sem_t * sem = arg->sem;

  sem_wait(sem);

  int reg = *i;
  // interupt thread
  sleep(2);
  reg --;
  *i = reg;

  sem_post(sem);
  pthread_exit(i);
}

void main() {
  int i = 65;

  // create semaphore
  sem_t sem;
  sem_init(&sem, 0, 1);

  // create threads
  pthread_t th1, th2;
  struct arg *arg = (struct arg *)malloc(sizeof(struct arg));
  arg->i = &i, arg->sem = &sem;

  pthread_create(&th1, NULL, decrement, (void *) arg);
  sleep(1);
  pthread_create(&th2, NULL, increment, (void *) arg);
  
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  printf("%i\n", i);

  return;
}