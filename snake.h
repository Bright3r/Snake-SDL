
#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdlib.h>

#define SNAKE_WIDTH 10

enum directions {Up, Down, Left, Right};

typedef struct snake {
  queue *body;
  int x, y;
  enum directions direction;
} snake;

snake *createSnake(int x, int y);
void growSnakeBody(snake *s);
void drawSnake(SDL_Renderer *renderer, snake *s);

#endif
