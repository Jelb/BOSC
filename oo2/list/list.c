/******************************************************************************
   list.c

   Implementation of simple linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "list.h"

/* list_new: return a new list structure */
List *list_new(void)
{
  List *l;

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
    Node *newlast = n;
    Node *oldlast = l->last;
    oldlast->next = newlast;
    while(n->next != NULL) {
        newlast = newlast->next;
        nlen++;
    }
    l->last = newlast;
    l->len += nlen;
}

/* list_remove: remove and return the first (non-root) element from list l */
Node *list_remove(List *l)
{
    Node *root = l->first;
    if(root->next == NULL)
        return NULL;
    
    Node *rm = root->next;
    root->next = rm->next;
    l->len -=1;
    rm->next = NULL;
  return rm;
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

/*int main(int argc, char *argv[]){
    List *l = list_new();
    int i;
    for (i = 0; i < argc; i++){
        list_add(l, node_new_str(argv[i]));
    }
    
    Node *node = l->first;
    i = 0;
    while (node->next != NULL) {
        printf("note: %i; %s\n",i,(char *)node->elm);
    }
    return 1;
}
*/




