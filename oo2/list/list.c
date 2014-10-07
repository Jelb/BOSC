/******************************************************************************
   list.c

   Implementation of simple linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

pthread_mutex_t mutex;

/* list_new: return a new list structure */
List *list_new(void)
{
  List *l;
    
  if (pthread_mutex_init(&mutex, NULL) != 0)
    {
      printf("Could not initialize mutex.\nOperation aborted...\n");
      return NULL;
    }
    
  l = (List *) malloc(sizeof(List));
  l->len = 0;

  /* insert root element which should never be removed */
  l->first = l->last = (Node *) malloc(sizeof(Node));
  l->first->elm = NULL;
  l->first->next = NULL;
  return l;
}

/* list_add: add node n to list l as the last element */
void list_add(List *l, Node *n)
{
    int nlen = 1;
    pthread_mutex_lock(&mutex);
    //sleep(1); //- Used for testing;
    Node *newlast = n;
    Node *oldlast = l->last;
    oldlast->next = newlast;
    while(n->next != NULL) {
        newlast = newlast->next;
        nlen++;
    }
    l->last = newlast;
    l->len += nlen;
    pthread_mutex_unlock(&mutex);
}

/* list_remove: remove and return the first (non-root) element from list l */
Node *list_remove(List *l)
{
    pthread_mutex_lock(&mutex);
    Node *root = l->first;
    if(root->next == NULL)
        return NULL;
    
    Node *rm = root->next;
    root->next = rm->next;
    l->len -=1;
    rm->next = NULL;
    pthread_mutex_unlock(&mutex);
  return rm;
}

void list_destroy()
{
    Node *n = l->first;
    while(n->next != NULL)
    {
        Node *rm = n;
        n = n->next;
        free(rm);
    }
    
    pthread_mutex_destroy(mutex);
    l = NULL;
}

/* node_new: return a new node structure */
Node *node_new(void)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = NULL;
  n->next = NULL;
  return n;
}

/* node_new_str: return a new node structure, where elm points to new copy of s */
Node *node_new_str(char *s)
{
  Node *n;
  n = (Node *) malloc(sizeof(Node));
  n->elm = (void *) malloc((strlen(s)+1) * sizeof(char));
  strcpy((char *) n->elm, s);
  n->next = NULL;
  return n;
}