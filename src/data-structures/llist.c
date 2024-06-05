

#include <stdlib.h>

struct node {
  struct node *next;
  void *data;
};

struct node *createNode(void *data) {
  struct node *curr = (struct node *) malloc(sizeof(struct node));
  curr->data = data;
  curr->next = NULL;

  return curr;
}

void destroyNode(struct node *curr) {
  free(curr->data);
  free(curr);
}

void insertNodeAfter(struct node *before, struct node *after) {
  after->next = before->next;
  before->next = after;
}
