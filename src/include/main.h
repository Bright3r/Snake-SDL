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
#include "apple.h"
#include "snake.h"
#include "queue.h"
#include "ui.h"
#include "network.h"
#include "utils.h"

#define GRID_ROW_SIZE 20
#define FRAME_INTERVAL 100.0f

enum game_modes {SinglePlayer, Host, Join};
enum end_screen_decisions {Quit, Restart};

void init();
int getCenter();
enum game_modes startScreen(SDL_Renderer *renderer);
bool isMultiplayer(enum game_modes game_mode);
void hostLobby(SDL_Renderer *renderer, int *sock_fd);
void joinLobby(SDL_Renderer *renderer, int *sock_fd);
void drawScore(SDL_Renderer *renderer, int score);
void drawGrid(SDL_Renderer *renderer);
enum end_screen_decisions endScreen(SDL_Renderer *renderer, int score, enum game_modes game_mode, enum game_stats game_status);
char *getTextInput(SDL_Renderer *renderer, char *promptText, int inputSize);
enum end_screen_decisions gameLoop(SDL_Renderer *renderer, enum game_modes game_mode, pthread_t *thread, pthread_mutex_t *lock, int sock_fd);
void hostExistingLobby(SDL_Renderer *renderer, int sock_fd);
void waitForHostToStart(int sock_fd);

#endif
