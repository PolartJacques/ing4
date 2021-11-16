#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t sem_vi, sem_firefox, sem_emacs;

void *startVi() {
  sem_wait(&sem_vi);
  system("vi");
  sem_post(&sem_firefox);
  pthread_exit(NULL);
}

void *startFirefox() {
  sem_wait(&sem_firefox);
  system("firefox");
  sem_post(&sem_emacs);
  pthread_exit(NULL);
}

void *startEmacs() {
  sem_wait(&sem_emacs);
  system("emacs");
  pthread_exit(NULL);
}

void main () {
  sem_init(&sem_vi, 0, 1);
  sem_init(&sem_firefox, 0, 0);
  sem_init(&sem_emacs, 0, 0);

  pthread_t th1, th2, th3;
  pthread_create(&th1, NULL, startVi, NULL);
  pthread_create(&th2, NULL, startFirefox, NULL);
  pthread_create(&th3, NULL, startEmacs, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  return;
}