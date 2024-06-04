#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "queue.c"
#include "utils.c"
#include "apple.c"
#include "snake.c"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 100.0f

void refreshScreen(SDL_Renderer *renderer);
void drawScore(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int windowWidth, int score);
void drawGrid(SDL_Renderer *renderer, int windowWidth);

#endif
