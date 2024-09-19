#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>

struct node {
  struct node *next;
  void *data;
};

struct node *createNode(void *data);
void destroyNode(struct node *curr);
void insertNodeAfter(struct node *before, struct node *after);

#endif
