#ifndef APPLE_H
#define APPLE_H

#include <SDL2/SDL.h>
#include <stdlib.h>

typedef SDL_Rect apple;

apple *createApple(int gridRowSize, int snakeWidth);
void destroyApple(apple *app);
void drawApple(SDL_Renderer *renderer, apple *app);
void moveApple(apple *app, int gridRowSize, int snakeWidth);

#endif
