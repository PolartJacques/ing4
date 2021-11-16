#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

struct arg {
  float a;
  float b;
  pthread_t thToAwait;
};

// function for thread 1
void * th1Function(void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  float *resTh1 = malloc(sizeof(float));
  float *resTh2;

  float adition = arg->a + arg->b;
  pthread_join(arg->thToAwait, (void **) &resTh2);
  *resTh1 = *resTh2 + adition;

  pthread_exit(resTh1);
}

// function for thread 2
void * th2Function(void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  float *resTh2 = malloc(sizeof(float));
  float *resTh3;

  float adition = arg->a + arg->b;
  pthread_join(arg->thToAwait, (void **) &resTh3);
  *resTh2 = adition - *resTh3;

  pthread_exit(resTh2);
}

// function for thread 3
void * th3Function(void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  float *resTh3 = malloc(sizeof(float));
  float *resTh4;

  float multiplication = arg->a * arg->b;
  pthread_join(arg->thToAwait, (void **) &resTh4);
  *resTh3 = multiplication / *resTh4;

  pthread_exit(resTh3);
}

// function for thread 4
void * th4Function(void * _arg) {
  struct arg *arg = (struct arg *) _arg;
  float *resTh4 = malloc(sizeof(float));
  *resTh4 = arg->a - arg->b;
  pthread_exit(resTh4);
}

void runThread(bool showResult) {
  // create threads
  pthread_t th1, th2, th3, th4;

  // create argument for earch thread function
  struct arg *arg1 = (struct arg *)malloc(sizeof(struct arg));
  struct arg *arg2 = (struct arg *)malloc(sizeof(struct arg));
  struct arg *arg3 = (struct arg *)malloc(sizeof(struct arg));
  struct arg *arg4 = (struct arg *)malloc(sizeof(struct arg));
  
  // thread 4 (e-f)
  arg4->a = 5; arg4->b = 6;
  pthread_create(&th4, NULL, th4Function, (void *) arg4);
  // thread 3 ((c*d) / result thread 4)
  arg3->a = 3; arg3->b = 4; arg3->thToAwait = th4;
  pthread_create(&th3, NULL, th3Function, (void *) arg3);
  // thread 2 ((a+b) - result thread 3)
  arg2->a = 1; arg2->b = 2; arg2->thToAwait = th3;
  pthread_create(&th2, NULL, th2Function, (void *) arg2);
  // thread 1 (result thread 2 + (g+h))
  arg1->a = 7; arg1->b = 8; arg1->thToAwait = th2;
  pthread_create(&th1, NULL, th1Function, (void *) arg1);

  // get the final result
  float *res;
  pthread_join(th1, (void **) &res);
  if (showResult) printf("%f\n", *res);
}

/*void main() {
  runThread(true);
  return;
}*/