#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oo2_1.h"

double sum;					/* this data is shared by the thread(s) */
void *runner(void *param);	/* threads call this function */

int main(int argc, char *argv[])
{
  pthread_attr_t attr;	/* set of thread attributes */

  if (argc != 3) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "N must be >= 0\n");
    return -1;
  }
  if (atoi(argv[2]) < 1) {
    fprintf(stderr, "Threads must be >= 0\n");
    return -1;
  }
  pthread_attr_init(&attr);	/* get the default attributes */
	
  pthread_t *pthread_ids;
  pthread_ids = (pthread_t *) malloc(atoi(argv[2]) * sizeof(pthread_t));
  Sumjob *jobs;
  jobs = (Sumjob *) malloc(atoi(argv[2]) * sizeof(Sumjob));

  int interval = atoi(argv[1]) / atoi(argv[2]);
  int i;

  for (i = 0; i < atoi(argv[2]); i++) {
    jobs[i].first = interval*i;
    jobs[i].last = ((i == atoi(argv[2])-1) ? atoi(argv[1]) : interval*(i+1));
    pthread_create(&pthread_ids[i],&attr,runner,&jobs[i]);	/* create the thread */
  }

  for (i = 0; i < atoi(argv[2]); i++) {
    pthread_join(pthread_ids[i],NULL);	/* wait for the thread to exit */
  }

  free(pthread_ids);
  free(jobs);
	
  printf("Threads: %i Sum = %f\n",atoi(argv[2]) ,sum);
}

/* The thread will begin control in this function */
void *runner(void *param)
{
  Sumjob *job = param;
  double localsum = 0;
  int i;
  for (i = job->first+1; i <= job->last; i++)
    localsum += sqrt(i);
    
  sum += localsum;

  pthread_exit(0);
}
