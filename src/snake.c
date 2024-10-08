#include "snake.h"

snake *createSnake(int x, int y) {
  snake *s = (snake *) malloc(sizeof(snake));

  s->body = createQueue();
  s->direction = Right;
  s->x = x;
  s->y = y;

  growSnakeBody(s); // Start with size 1
  return s;
}

void destroySnake(snake *s) {
  destroyQueue(s->body);
  free(s);
}

void drawSnake(SDL_Renderer *renderer, snake *s) {
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
  
  // Iterate through rectangles of snake body
  struct node *curr = s->body->head;
  for (int i = 0; i < s->body->size; i++) {
    SDL_Rect *body = (SDL_Rect *) curr->data;
    if (body != NULL) {
      SDL_RenderFillRect(renderer, body);
    }

    curr = curr->next;
  }
}

void growSnakeBody(snake *s) {
  SDL_Rect *rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  rect->x = s->x;
  rect->y = s->y;
  rect->w = SNAKE_WIDTH;
  rect->h = SNAKE_WIDTH;

  push(s->body, (void *) rect);
}

void moveSnake(snake *s) {
  growSnakeBody(s); // add rect to new head location
  void *data = pop(s->body); // remove tail
  free(data);
}

void updateSnakePosition(snake *s) {
  switch (s->direction) {
    case Up:
      s->y += -SNAKE_WIDTH;
      break; 
    case Down:
      s->y += SNAKE_WIDTH;
      break;
    case Left:
      s->x += -SNAKE_WIDTH;
      break;
    case Right:
      s->x += SNAKE_WIDTH;
      break;
  }
}

SDL_Rect *getSnakeHead(snake *s) {
  return (SDL_Rect *) s->body->tail->data;
}

struct node *getSnakeTailNode(snake *s) {
  return s->body->head;
}

bool checkSnakeBorderCollision(snake *s, int gridRowSize) {
  int max_x = gridRowSize * SNAKE_WIDTH;
  int max_y = max_x;

  if ((s->x + SNAKE_WIDTH) > max_x || (s->y + SNAKE_WIDTH) > max_y
    || s->x < 0 || s->y < 0) {
    return true;
  }
  return false;
}

bool checkSnakeSelfCollision(snake *s) {
  SDL_Rect *snakeHead = getSnakeHead(s);
  struct node *snakeBodySegment = getSnakeTailNode(s);

  // Iterate through all body rects and check if head is colliding
  for (int i = 1; i < s->body->size; i++) {
    if (checkCollision(snakeHead, (SDL_Rect *) snakeBodySegment->data)) {
      return true;
    }

    snakeBodySegment = snakeBodySegment->next;
  }

  return false;
}
