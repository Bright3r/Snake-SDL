
#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "utils.c"

#define SNAKE_WIDTH 25

enum directions {Up, Down, Left, Right};

typedef struct snake {
  queue *body;
  int x, y;
  enum directions direction;
} snake;

snake *createSnake(int x, int y);
void growSnakeBody(snake *s);
void drawSnake(SDL_Renderer *renderer, snake *s);
void moveSnake(snake *s);
void updateSnakePosition(snake *s);
SDL_Rect *getSnakeHead(snake *s);
struct node *getSnakeTailNode(snake *s);
bool checkSnakeBorderCollision(snake *s, int gridRowSize);
bool checkSnakeSelfCollision(snake *s);

#endif
