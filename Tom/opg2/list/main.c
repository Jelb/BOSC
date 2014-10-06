/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "list.h"
#include "testjob.h"

// FIFO list;
List *fifo;
pthread_attr_t attr;

void test();

int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    printf("###\n");
    if (strcmp(argv[1],"test") == 0)
    {
     printf("!!!\n");
     test();  
    }
  }

  printf("???\n");
  fifo = list_new();

  list_add(fifo, node_new_str("s1"));
  list_add(fifo, node_new_str("s2"));
  
  printf("Length:%d\n", get_length(fifo));

  Node *n1 = list_remove(fifo);
  if (n1 == NULL) { printf("Error no elements in list\n"); exit(-1);}
  Node *n2 = list_remove(fifo);
  if (n2 == NULL) { printf("Error no elements in list\n"); exit(-1);}

  printf("%s\n%s\n", n1->elm, n2->elm);
  printf("Length:%d\n", get_length(fifo)); 

  return 0; 

}


void test()
{
  List *fifotest;

  fifotest = list_new();

  pthread_attr_init(&attr);
  
  pthread_t pthread_ids[20];
  pthread_ids = (pthread_t *) malloc(20 * sizeof(pthread_t));


  printf("done allocating memory for thread ids\n");

  Testjob testjobs[20];
  testjobs = (Testjob *) malloc(20 * sizeof(Testjob));


  printf("done allocating memory for testjobs\n");

  int i;

  for (i = 0; i < 10; i++)
  {
    testjobs[i].n = i;
    testjobs[i].l = fifotest;
    pthread_create(&pthread_ids[i],&attr,test_add,&testjobs[i]);
    pthread_create(&pthread_ids[i+10],&attr,list_remove,&fifotest);
  }

  printf("done starting threads\n");

  for (i = 0; i < 20; i++)
  {
    pthread_join(pthread_ids[i],NULL);
  }


  printf("done joining threads\n");

  //free(pthread_ids);
  //free(testjobs);

}