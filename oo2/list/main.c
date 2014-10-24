/*************************************************************************
   main.c
   Implementation of a simple FIFO buffer as a linked list defined in list.h.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"


// FIFO list;
List *fifo;
void *add(void *param);
void *testremove(void *param);
int test();

int main(int argc, char* argv[])
{
  fifo = list_new();
  test();
  return 0;
}

void test() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  int i, j;
  fifo = list_new();
    
  pthread_t ids[2];
  pthread_create(&ids[0],&attr,add,"thread 0");
  pthread_create(&ids[1],&attr,add,"thread 1");
    
  for (j = 0; j < 2; j++) 
    pthread_join(ids[j],NULL);	/* wait for the thread to exit */
  
  for (i = 0; i < 20; i++) {
    Node *n = list_remove(fifo);
    printf("Removed: %s\n",(char *)n->elm);
  }
}

void *add(void *param)
{
  int i;
  for (i = 0; i < 10; i++) {
    list_add(fifo,node_new_str((char *)param));
    printf("added: %s\n",(char *)param);
  }
  pthread_exit(0);
}
