#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./data-structures/queue.c"
#include "./objs/apple.c"
#include "./objs/snake.c"
#include "network.c"
#include "utils.c"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 100.0f

enum game_modes {SinglePlayer, MultiPlayer};

void refreshScreen(SDL_Renderer *renderer);
void startScreen(SDL_Renderer *renderer, TTF_Font *font, int windowWidth, enum game_modes *game_mode, int *sock_fd);
void hostLobby(int *sock_fd);
void joinLobby(int *sock_fd);
void drawText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, char *msg);
void drawScore(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int windowWidth, int score);
void drawGrid(SDL_Renderer *renderer, int windowWidth);
void endScreen(SDL_Renderer *renderer, TTF_Font *font, int windowWidth, int score);

#endif
