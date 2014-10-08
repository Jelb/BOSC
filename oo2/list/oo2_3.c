#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphores.h>

void *producer(void *param);
void *comsumer(void *param);

int n;
sem_t *mutex, *full, *empty;

/*
  Call the program with the following parameters:
  Producers (p)
  Consumers (c)
  Buffer size (b)
  Number of products to produce (n)
*/
int main(int argc, char *argv[]) {
    
    int p,c,b,n;

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
      p = atoi(argv[2]);
    }

    if (atoi(argv[3]) < 1) {
      printf("buffer size must be positive number\n");
      exit(0);
    } else {
      p = atoi(argv[3]);
    }

    if (atoi(argv[4]) < 1) {
      printf("number of products to produce must be positive\n");
      exit(0);
    } else {
      p = atoi(argv[4]);
    }

    if (sem_init(mutex, 0, 1) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(full, 0, 0) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(empty, 0, b) != 0) {
    /*ERROR*/
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
