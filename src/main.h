#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "./data-structures/queue.c"
#include "./objs/apple.c"
#include "./objs/snake.c"
#include "ui.c"
#include "network.c"
#include "utils.c"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 100.0f

enum game_modes {SinglePlayer, Host, Join};

void init();
enum game_modes startScreen(SDL_Renderer *renderer);
bool isMultiplayer(enum game_modes game_mode);
void hostLobby(SDL_Renderer *renderer, int *sock_fd);
void joinLobby(SDL_Renderer *renderer, int *sock_fd);
void drawScore(SDL_Renderer *renderer, int score);
void drawGrid(SDL_Renderer *renderer);
void endScreen(SDL_Renderer *renderer, int score, enum game_stats game_status);
void gameLoop(SDL_Renderer *renderer, enum game_modes game_mode, pthread_t *thread, pthread_mutex_t *lock, int sock_fd);

#endif
