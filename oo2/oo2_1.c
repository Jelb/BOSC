#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "oo2_1.h"

int sum;					/* this data is shared by the thread(s) */
void *runner(void *param);	/* threads call this function */

int main(int argc, char *argv[])
{
	pthread_t tid; 			/* the thread identifier */
	pthread_attr_t attr;	/* set of thread attributes */

	if (argc != 3)
	{
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}

	if (atoi(argv[1]) < 0)
	{
		fprintf(stderr, "N must be >= 0\n");
		return -1;
	}

	if (atoi(argv[2]) < 1)
	{
		fprintf(stderr, "Threads must be >= 0\n");
		return -1;
	}

	pthread_attr_init(&attr);					/* get the default attributes */
	
	pthread_t *pthread_ids;
	pthread_ids = (pthread_t *) malloc(atoi(argv[2]) * sizeof(pthread_t));

	int first = 0;
	int last = 0;
	int interval = atoi(argv[1]) / atoi(argv[2]);
	int i;

	for (i = 0; i < atoi(argv[2]); i++)
	{
		first = last;
		last = last + interval;
		Sumjob job;
		job.first = first*i;
		job.last = last*++i;
		pthread_create(&pthread_ids[i],&attr,runner,&job);	/* create the thread */
	}

	for (i = 0; i < atoi(argv[2]); i++)
	{
		pthread_join(pthread_ids[i],NULL);						/* wait for the thread to exit */
	}
	
	printf("sum = %d\n",sum);
}

/* The thread will begin control in this function */
void *runner(void *param)
{
	Sumjob *job = param;
	int first = job->first;
	int last  = job->last;
	int localsum = 0;

	for (first; first < last; first++)
		localsum += first;

	sum += localsum;

	pthread_exit(0);
}
