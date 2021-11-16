#include <sys/times.h>
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>

#include "process.c"
#include "thread.c"


clock_t times(struct tms *buf);

void main() 
{
  struct tms start, end;
  struct rusage rstart, rend;
  int i;

  // PROCESS
  printf("PROCESS :\n");
  times(&start);
  getrusage(RUSAGE_SELF, &rstart);

   for (i=0; i<1000; i++)
    runProcess(false);

  times(&end);
  getrusage(RUSAGE_SELF, &rend);
  printf("%lf usec\n", (end.tms_utime+end.tms_stime-start.tms_utime-start.tms_stime)*1000000.0/sysconf(_SC_CLK_TCK));
  printf("%ld usec\n", (rend.ru_utime.tv_sec-rstart.ru_utime.tv_sec)*1000000 +(rend.ru_utime.tv_usec-rstart.ru_utime.tv_usec)+(rend.ru_stime.tv_sec-rstart.ru_stime.tv_sec)*1000000 +(rend.ru_stime.tv_usec-rstart.ru_stime.tv_usec));

  // THREAD
  printf("\nTHREAD :\n");
  times(&start);
  getrusage(RUSAGE_SELF, &rstart);

   for (i=0; i<1000; i++)
    runThread(false);

  times(&end);
  getrusage(RUSAGE_SELF, &rend);
  printf("%lf usec\n", (end.tms_utime+end.tms_stime-start.tms_utime-start.tms_stime)*1000000.0/sysconf(_SC_CLK_TCK));
  printf("%ld usec\n", (rend.ru_utime.tv_sec-rstart.ru_utime.tv_sec)*1000000 +(rend.ru_utime.tv_usec-rstart.ru_utime.tv_usec)+(rend.ru_stime.tv_sec-rstart.ru_stime.tv_sec)*1000000 +(rend.ru_stime.tv_usec-rstart.ru_stime.tv_usec));

  return;
}
