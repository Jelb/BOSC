#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "list.h"

void *producer(void *param);
void *consumer(void *param);

int n, m;
sem_t *mutex, *full, *empty;

/*
  Call the program with the following parameters:
  Producers (p)
  Consumers (c)
  Buffer size (b)
  Number of products to produce (n)
*/
int main(int argc, char *argv[]) {
    
  int p,c,b;

  if (atoi(argv[1]) < 1) {
    printf("producers must be positive number\n");
    exit(0);
  } else {
    p = atoi(argv[1]);
  }

  if (atoi(argv[2]) < 1) {
    printf("consumers must be positive number\n");
    exit(0);
  } else {
    c = atoi(argv[2]);
  }

  if (atoi(argv[3]) < 1) {
    printf("buffer size must be positive number\n");
    exit(0);
  } else {
    b = atoi(argv[3]);
  }

  if (atoi(argv[4]) < 1) {
    printf("number of products to produce must be positive\n");
    exit(0);
  } else {
    m = n = atoi(argv[4]);
  }

  if (sem_init(mutex, 0, 1) != 0) {
    printf("Unable to initialize semaphors");
  }

  if (sem_init(full, 0, 0) != 0) {
    printf("Unable to initialize semaphors");
  }

  if (sem_init(empty, 0, b) != 0) {
    printf("Unable to initialize semaphors");
  }

  int i;

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_t *cids;
  cids = (pthread_t *) malloc(c * sizeof(pthread_t));

  for (i = 0; i < c; i++) {
    pthread_create(&cids[i],&attr,consumer,NULL);
  }
      
  pthread_t *pids;
  pids = (pthread_t *) malloc(p * sizeof(pthread_t));

  for (i = 0; i < p; i++) {
    pthread_create(&pids[i],&attr,producer,NULL);
  }
}

void *producer(void *param) {
  do {
    
  } while(n > 0);
}


void *consumer(void *param) {
  do {
    
  } while(m > 0 );
}
