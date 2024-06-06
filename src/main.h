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
#include "ui.c"
#include "network.c"
#include "utils.c"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 100.0f

enum game_modes {SinglePlayer, MultiPlayer};

void refreshScreen(SDL_Renderer *renderer);
void startScreen(SDL_Renderer *renderer, int windowWidth, enum game_modes *game_mode, int *sock_fd);
void hostLobby(SDL_Renderer *renderer, int *sock_fd);
void joinLobby(int *sock_fd);
void drawScore(SDL_Renderer *renderer, int windowWidth, int score);
void drawGrid(SDL_Renderer *renderer, int windowWidth);
void endScreen(SDL_Renderer *renderer, int windowWidth, int score);

#endif
