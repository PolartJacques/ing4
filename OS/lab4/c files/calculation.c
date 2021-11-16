#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;

int *sharedArray; // store result of thread
int fas = 0; // first available spot
sem_t sem; // protect shared values : sharedArray, fas
sem_t semResult; // use to count available results from threads

// a + b
void * thread1() {
  int res = a + b;
  sem_wait(&sem);
  sharedArray[fas] = res;
  fas++;
  sem_post(&semResult);
  sem_post(&sem);
  pthread_exit(NULL);
}

// c - d
void * thread2() {
  int res = c - d;
  sem_wait(&sem);
  sharedArray[fas] = res;
  fas++;
  sem_post(&semResult);
  sem_post(&sem);
  pthread_exit(NULL);
}

// e + f
void * thread3() {
  int res = e + f;
  sem_wait(&sem);
  sharedArray[fas] = res;
  fas++;
  sem_post(&semResult);
  sem_post(&sem);
  pthread_exit(NULL);
}

// multiplie the result of thread 1, 2, 3
void * thread4() {
  // init response variable
  int *res = malloc(sizeof(int));
  // whait for 2 results to be saved in the sharded array
  sem_wait(&semResult);
  sem_wait(&semResult);
  // make the first multiplication
  *res = sharedArray[0] * sharedArray[1];
  // wait for the third results to be save in the sharded array
  sem_wait(&semResult);
  // make and send the second multiplication
  *res = *res * sharedArray[2];
  pthread_exit(res);
}

void main() {
  // init variables
  sharedArray = (int *) malloc(sizeof(int) * 3);
  sem_init(&sem, 0, 1);
  sem_init(&semResult, 0, 0);

  // create threads
  pthread_t th1, th2, th3, th4;
  pthread_create(&th1, NULL, thread1, NULL);
  pthread_create(&th2, NULL, thread2, NULL);
  pthread_create(&th3, NULL, thread3, NULL);
  pthread_create(&th4, NULL, thread4, NULL);

  // get the result from thread 4
  int *res;
  pthread_join(th4, (void **) &res);
  printf("%i\n", *res);

  // destroy semaphores
  sem_destroy(&sem);
  sem_destroy(&semResult);
  return;
}