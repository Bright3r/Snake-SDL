#include "main.h"

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    fprintf(stderr, "SDL_Init has failed: %s\n", SDL_GetError());
  }

  int windowWidth = GRID_ROW_SIZE * SNAKE_WIDTH;
  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,windowWidth, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to render window: %s\n", SDL_GetError());
  }

  bool isGameRunning = true;
  SDL_Event event;
  snake *s = createSnake(50, 50);
  while (isGameRunning) {
    uint32_t startTime = SDL_GetTicks();

    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        isGameRunning = false;
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_w:
            s->direction = Up;
            break;
          case SDLK_a:
            s->direction = Left;
            break;
          case SDLK_s:
            s->direction = Down;
            break;
          case SDLK_d:
            s->direction = Right;
            break;
        }
      }
    }

    updateSnakePosition(s);

    refreshScreen(renderer);
    drawSnake(renderer, s);

    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime)); // cap framerate
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void refreshScreen(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}


// bool checkCollision(SDL_Rect *r1, SDL_Rect *r2) {
//
// }
