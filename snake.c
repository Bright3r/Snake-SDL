
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

  SDL_RenderPresent(renderer);
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
  pop(s->body); // remove tail
}

void updateSnake(snake *s) {
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

  moveSnake(s);
}
