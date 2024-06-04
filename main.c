#include "main.h"

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL_Init has failed: %s\n", SDL_GetError());
  }

  if (TTF_Init() < 0) {
    fprintf(stderr, "TTF_Init has failed: %s\n", TTF_GetError());
  }

  TTF_Font *font = TTF_OpenFont("./assets/fonts/PixelifySans-VariableFont_wght.ttf", 24);
  if (font == NULL) {
    fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
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

  // Initialize game objects
  snake *s = createSnake(50, 50);
  apple *app = createApple(GRID_ROW_SIZE, SNAKE_WIDTH);
  int score = 0;

  // Game Loop
  bool isGameRunning = true;
  SDL_Event event;
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

    // Check if snake is colliding with border or with self
    if (checkSnakeBorderCollision(s, GRID_ROW_SIZE) || checkSnakeSelfCollision(s)) {
      isGameRunning = false;
    }

    // Check if snake is colliding with apple
    if (checkCollision(getSnakeHead(s), app)) {
      growSnakeBody(s);
      score++;
      printf("Score: %d\n", score);

      moveApple(app, GRID_ROW_SIZE, SNAKE_WIDTH);
    }
    else {
      moveSnake(s);
    }

    // Draw updated objects
    refreshScreen(renderer);
    drawApple(renderer, app);
    drawSnake(renderer, s);

    SDL_Color color = {255, 255, 255, 255};
    drawScore(renderer, font, color, windowWidth, score);

    SDL_RenderPresent(renderer);

    // Cap Framerate
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime));
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

void drawScore(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int windowWidth, int score) {
  // Get string of score
  char scoreText[12] = {0};
  sprintf(scoreText, "Score: %d", score);

  SDL_Surface *text_surface = TTF_RenderText_Solid(font, scoreText, color);
  SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);

  int midpoint = (windowWidth - text_surface->w) / 2;
  SDL_Rect dest = {midpoint, text_surface->h, text_surface->w, text_surface->h};
  SDL_RenderCopy(renderer, text, NULL, &dest);

  SDL_DestroyTexture(text);
  SDL_FreeSurface(text_surface);
}
