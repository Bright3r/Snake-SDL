#include "queue.h"

queue *createQueue() {
  queue *q = (queue *) malloc(sizeof(queue));
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;

  return q;
}

void destroyQueue(queue *q) {
  struct node *curr = q->head;
  for (int i = 0; i < q->size; i++) {
    struct node *temp = curr->next;
    destroyNode(curr);
    curr = temp;
  }
  free(q);
}

void push(queue *q, void *data) {
  if (q->head == NULL) {  // Create linked list of size 1
    q->head = createNode(data);
    q->tail = q->head;
  }
  else {  // Add new node after tail
    insertNodeAfter(q->tail, createNode(data));
    q->tail = q->tail->next;
  }
  q->size++;
}

void *pop(queue *q) {
  if (q->head == NULL) {
    return NULL;
  }

  // Iterate head of llist
  void *data = q->head->data;
  struct node *temp = q->head;

  // Remove old head of llist
  q->head = q->head->next;
  free(temp);
  q->size--;

  if (q->size == 0) { // If removed head was also tail, remove dangling pointer
    q->tail = NULL;
  }

  return data;
}
