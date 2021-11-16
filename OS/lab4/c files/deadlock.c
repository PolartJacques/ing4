
// RESOLVE SYNCHRONISATION ERROR WITH SEMAPHORE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

sem_t sem_recource1, sem_recource2, sem_recource3;

/**
 * hold ressource 1
 * sleep
 * wait for ressource 2
 */
void * thread1 (void * _arg) {
  printf("thread 1 started !\n");
  // hold resource 1
  sem_wait(&sem_recource1);
  // dangerous sleep
  sleep(1);
  // then hold recource 2
  sem_wait(&sem_recource2);
  // release the ressources
  sem_post(&sem_recource2);
  sem_post(&sem_recource1);
  // end thread
  printf("thread 1 finish !\n");
  pthread_exit(NULL);
}

/**
 * hold ressource 2
 * sleep
 * wait for ressource 3
 */
void * thread2 (void * _arg) {
  printf("thread 2 started !\n");
  // hold resource 2
  sem_wait(&sem_recource2);
  // dangerous sleep
  sleep(1);
  // then hold recource 3
  sem_wait(&sem_recource3);
  // release the ressources
  sem_post(&sem_recource3);
  sem_post(&sem_recource2);
  // end thread
  printf("thread 2 finish !\n");
  pthread_exit(NULL);
}

/**
 * hold ressource 3
 * sleep
 * wait for ressource 1
 */
void * thread3 (void * _arg) {
  printf("thread 3 started !\n");
  // hold resource 3
  sem_wait(&sem_recource3);
  // dangerous sleep
  sleep(1);
  // then hold recource 1
  sem_wait(&sem_recource2);
  // release the ressources
  sem_post(&sem_recource1);
  sem_post(&sem_recource3);
  // end thread
  printf("thread 3 finish !\n");
  pthread_exit(NULL);
}

void main() {
  // init semaphores
  sem_init(&sem_recource1, 0, 1);
  sem_init(&sem_recource2, 0, 1);
  sem_init(&sem_recource3, 0, 1);

  // create threads
  pthread_t th1, th2, th3;
  pthread_create(&th1, NULL, thread1, NULL);
  pthread_create(&th2, NULL, thread2, NULL);
  pthread_create(&th3, NULL, thread3, NULL);
  
  // wait for threads to finish (not gonna happened)
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);

  return;
}