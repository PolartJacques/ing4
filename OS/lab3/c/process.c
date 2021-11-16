#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#define KEY 4567
#define PERM 0660

void runProcess(bool showResult) {
  int memory = shmget(KEY, sizeof(float), IPC_CREAT | PERM);
  float *value;
  value = (float *) shmat(memory, NULL, 0);
  *value = 0;
  

  if (fork() == 0)
  {
    if (fork() == 0)
    {
      if (fork() == 0)
      {
        *value = 3 * 4;
        exit(0);
      }
      else
      {
        float res = 5 - 6;
        wait(NULL);
        *value = *value / res;
        exit(0);
      }
    }
    else
    {
      float res = 1 + 2;
      wait(NULL);
      *value = res - *value;
      exit (0);
    }
  }
  else
  {
    float res = 7 + 8;
    wait(NULL);
    *value = *value + res;

    if (showResult) printf("%f\n", *value);
    shmctl(memory, IPC_RMID, NULL);
  }
}

/*void main() {
  runProcess(true);
  return;
}*/