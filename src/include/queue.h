#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "llist.h"

typedef struct queue {
  int size;
  struct node *head;
  struct node *tail;
} queue;

queue *createQueue();
void destroyQueue(queue *q);
void push(queue *q, void *data);
void *pop(queue *q);

#endif
