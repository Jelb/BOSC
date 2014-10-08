#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "list.h"

void *producer(void *param);
void *consumer(void *param);
void Sleep(float wait_time_ms);

/* n is remaining number to be produced, m is remaining number to be consumed */
int n, m, o, b;

sem_t *mutex, *full, *empty;
List *buf;

/*
  Call the program with the following parameters:
  Producers (p)
  Consumers (c)
  Buffer size (b)
  Number of products to produce (n)
*/
int main(int argc, char *argv[]) {
    
  int p,c;

  /* Parse and check input and store in variables */
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
    o = m = n = atoi(argv[4]);
  }

  /* Initiate semaphores */
  if (sem_init(mutex, 0, 1) != 0) {
    printf("Unable to initialize semaphors");
  }

  if (sem_init(full, 0, 0) != 0) {
    printf("Unable to initialize semaphors");
  }

  if (sem_init(empty, 0, b) != 0) {
    printf("Unable to initialize semaphors");
  }

  buf = list_new();
  
  int i;

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_t *cids;
  cids = (pthread_t *) malloc(c * sizeof(pthread_t));

  for (i = 0; i < c; i++) {
    pthread_create(&cids[i],&attr,consumer,&i);
  }
      
  pthread_t *pids;
  pids = (pthread_t *) malloc(p * sizeof(pthread_t));

  for (i = 0; i < p; i++) {
    pthread_create(&pids[i],&attr,producer,&i);
  }
  
  for (i = 0; i < p; i++) {
    pthread_join(pids[i], NULL);
  }
  
  for (i = 0; i < c; i++) {
    pthread_join(cids[i], NULL);
  }
  
  sem_destroy(mutex);
  sem_destroy(full);
  sem_destroy(empty);
  free(buf);
}

void *producer(void *param) {
  
  int producer_no;
  producer_no = *(int *)param;

  do {

    n = n - 1;

    // produce an item
    Sleep(1000);
    Node *node;
    char *item_name = "Item_x";

    node = node_new_str(item_name);

    sem_wait(empty);
    sem_wait(mutex);

    // add to buffer 

    list_add(buf, node);
    printf("Producer %d produced %s.\n", producer_no, item_name);

    sem_post(mutex);
    sem_post(full);
    
  } while(n > 0);

  pthread_exit(0);
}


void *consumer(void *param) {
  int id = atoi(param);
  int bufc;
  do {
    m = m - 1;
    sem_wait(full);
    sem_wait(mutex);
    
    Node *n = list_remove(buf);
    sem_getvalue(full, &bufc);
    sem_post(mutex);
    sem_post(empty);
    
    printf("Consumer %i comsumed %s. Items in buffer: %i (out of %i)", id, (char *)n->elm, bufc, b);
  } while(m > 0);
}

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}
