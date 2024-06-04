
#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdlib.h>

#define SNAKE_WIDTH 50

typedef struct snake {
  queue *body;
  int x, y;
} snake;

snake *createSnake(int x, int y);
void growSnakeBody(snake *s);
void drawSnake(SDL_Renderer *renderer, snake *s);

#endif
