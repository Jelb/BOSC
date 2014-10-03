#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sum.h"

double sum;					/* this data is shared by the thread(s) */
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

	Sumjob *sumjobs;
	sumjobs = (Sumjob *) malloc(atoi(argv[2]) * sizeof(Sumjob));

	int i;

	for (i = 0; i < atoi(argv[2]); i++)
	{
		/* not the 'last' thread */
		if (i < atoi(argv[2])-1)
		{
			sumjobs[i].first = i * atoi(argv[1]) / atoi(argv[2]);
			sumjobs[i].last = (i+1) * atoi(argv[1]) / atoi(argv[2]);
			pthread_create(&pthread_ids[i],&attr,runner,&sumjobs[i]);	/* create the thread */
		}

		/* the 'last' thread */
		else
		{
			sumjobs[i].first = i * atoi(argv[1]) / atoi(argv[2]);
			sumjobs[i].last  = atoi(argv[1])+1; 
			pthread_create(&pthread_ids[i],&attr,runner,&sumjobs[i]);	/* create the thread */
		}
	}

	for (i = 0; i < atoi(argv[2]); i++)
	{
		pthread_join(pthread_ids[i],NULL);						/* wait for the thread to exit */
	}
	
	printf("sum = %f\n",sum);

	free(pthread_ids);
	free(sumjobs);
}

/* The thread will begin control in this function */
void *runner(void *param)
{
	Sumjob *job = param;
	int first = job->first;
	int last  = job->last;
	double localsum = 0.0;

	for (first; first < last; first++)
	{
		localsum += sqrt((double)first) ;
	}

	sum += localsum;

	pthread_exit(0);
}
