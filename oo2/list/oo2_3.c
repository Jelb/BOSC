#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphores.h>
#include "list.h"

void *producer(void *param);
void *comsumer(void *param);

int b;
sem_t *mutex, *full, *empty;

int main(int argc, char *argv[]) {
    
    if (sem_init(mutex, 0, 1) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(full, 0, 0) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(empty, 0, b) != 0) {
    /*ERROR*/
    }
    
    int i;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_t *cids;
	cids = (pthread_t *) malloc(c * sizeof(pthread_t));
    
    for (i = 0; i < c; i++) {
        pthread_create(&c_ids[i],&attr,consumer,NULL);
    }
        
    pthread_t *pids;
	pids = (pthread_t *) malloc(p * sizeof(pthread_t));
	
	for (i = 0; i < p; i++) {
        pthread_create(&pids[i],&attr,producer,NULL);
    }
}

void *producer(void *param) {
  do {
    
  } while(true);
}


void *consumer(void *param) {
  do {
    
  } while(true);
}
