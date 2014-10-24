#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include "list/list.h"

void *producer(void *param);
void *consumer(void *param);
void Sleep(float wait_time_ms);
int do_produce();
int do_consume();
void cleanUp();

int produced, consumed, o, b, product_count;

sem_t _mutex, full, empty, sem_produced, sem_consumed, sem_count;
List *buf;

/*
  Call the program with the following parameters:
  Producers (p)
  Consumers (c)
  Buffer size (b)
  Number of products to produce (o)
*/
int main(int argc, char *argv[]) {
  int p,c;
  produced = consumed = 0;

  /* Parse and check input and store in variables */
  if ((p = atoi(argv[1])) < 1) {
    printf("producers must be positive number\n");
    exit(0);
  }
  if ((c = atoi(argv[2])) < 1) {
    printf("consumers must be positive number\n");
    exit(0);
  }
  if ((b = atoi(argv[3])) < 1) {
    printf("buffer size must be positive number\n");
    exit(0);
  }
  if ((o = atoi(argv[4])) < 1) {
    printf("number of products to produce must be positive\n");
    exit(0);
  }
  
  /* seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  
  /* Initiate semaphores */
  if (sem_init(&sem_produced, 0, 1) 
      || sem_init(&sem_consumed, 0, 1)
      || sem_init(&sem_count, 0, 1)
      || sem_init(&_mutex, 0, 1)
      || sem_init(&full, 0, 0)
      || sem_init(&empty, 0, b)) {
    printf("Unable to initialize semaphors.\n");
  }

  printf("You have requested:\n");
  printf("%4i producers producing a total of %6i items.\n", p, o);
  printf("%4i consumers and a buffer containing up to %6i elements\n", c, b);

  buf = list_new();
  int i;
  int *ids;
  ids = (int *) malloc((c > p ? c : p) * sizeof(int));
  
  for (i = 0; i < (c > p ? c : p); i++)
    ids[i] = i;

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_t *cids;
  cids = (pthread_t *) malloc(c * sizeof(pthread_t));
  
  for (i = 0; i < c; i++)
    pthread_create(&cids[i],&attr,consumer,&ids[i]);
      
  pthread_t *pids;
  pids = (pthread_t *) malloc(p * sizeof(pthread_t));

  for (i = 0; i < p; i++)
    pthread_create(&pids[i],&attr,producer,&ids[i]);
  
  for (i = 0; i < p; i++)
    pthread_join(pids[i], NULL);
  
  for (i = 0; i < c; i++)
    pthread_join(cids[i], NULL);
  
  printf("\nResult:\n");
  printf("%6i items has been produced.\n", produced);
  printf("%6i items has been consumed.\n", consumed);
  printf("%6i elements are left in buffer.\n", buf->len);
  
  cleanUp();
  exit(0);
}

/*Producer function*/
void *producer(void *param) {
  
  int id = *(int *)param;
  Node *node;
  char elm[10];

  while(do_produce()) {
    Sleep(1000);
    
    sem_wait(&sem_count);
    sprintf(elm,"Item_%i",product_count);
    product_count++;
    sem_post(&sem_count);
    
    node = node_new_str(elm);

    sem_wait(&empty);
    sem_wait(&_mutex);

    list_add(buf, node);
    printf("Producer %4i produced %9s. Items in buffer: %4i (out of %4i)\n", id, elm, buf->len, b);
    
    sem_post(&_mutex);
    sem_post(&full);    
  }

  pthread_exit(0);
}

/*Consumer function*/
void *consumer(void *param) {
  int id = *(int *)param;
  Node *n;
  
  while(do_consume()) {
    Sleep(1000);
    
    sem_wait(&full);
    sem_wait(&_mutex);
    
    n = list_remove(buf);
    
    sem_post(&_mutex);
    sem_post(&empty);
    
    printf("Consumer %4i comsumed %9s. Items in buffer: %4i (out of %4i)\n", id, (char *)n->elm, buf->len, b);
      
    free(n); 
  }
  
  pthread_exit(0);
}

/*Checks if more products needs to be produced*/
int do_produce(void) {
  sem_wait(&sem_produced);
  int r = (produced < o ? ++produced : 0);
  sem_post(&sem_produced);
  return r;
}

/*Checks if more products are to be consumed*/
int do_consume(void) {
  sem_wait(&sem_consumed);
  int r = (consumed < o ? ++consumed : 0);
  sem_post(&sem_consumed);
  return r;
}

/* Random sleep function */
void Sleep(float wait_time_ms) {
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

void cleanUp() {
  sem_destroy(&_mutex);
  sem_destroy(&full);
  sem_destroy(&empty);
  sem_destroy(&sem_produced);
  sem_destroy(&sem_consumed);
  sem_destroy(&sem_count);
  free(buf);
}