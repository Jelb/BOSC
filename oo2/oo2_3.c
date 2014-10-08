#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphores.h>

void *producer(void *param);
void *comsumer(void *param);

int n;
sem_t *mutex, *full, *empty;

int main(int argc, char *argv[]) {
    
    if (sem_init(mutex, 0, 1) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(full, 0, 0) != 0) {
    /*ERROR*/
    }
    
    if (sem_init(empty, 0, n) != 0) {
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
