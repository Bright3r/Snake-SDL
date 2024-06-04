#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "queue.c"
#include "snake.c"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 200.0f

void refreshScreen(SDL_Renderer *renderer);
bool checkCollision(SDL_Rect *r1, SDL_Rect *r2);

#endif
