
#include "snake.h"

snake *createSnake(int x, int y) {
  snake *s = (snake *) malloc(sizeof(snake));

  s->body = createQueue();
  s->x = x;
  s->y = y;

  growSnakeBody(s); // Start with size 1
  return s;
}

void growSnakeBody(snake *s) {
  SDL_Rect *rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
  rect->x = s->x;
  rect->y = s->y;
  rect->w = SNAKE_WIDTH;
  rect->h = SNAKE_WIDTH;

  push(s->body, (void *) rect);
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
