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
  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowWidth, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to render window: %s\n", SDL_GetError());
  }

  startScreen(renderer, font, windowWidth);

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
    drawGrid(renderer, windowWidth);

    SDL_Color color = {255, 255, 255, 255};
    drawScore(renderer, font, color, windowWidth, score);

    SDL_RenderPresent(renderer);

    // Cap Framerate
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime));
  }

  endScreen(renderer, font, windowWidth, score);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void refreshScreen(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 148, 148, 184, 255);
  SDL_RenderClear(renderer);
}

void startScreen(SDL_Renderer *renderer, TTF_Font *font, int windowWidth) {
  refreshScreen(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Color color = {255, 255, 255, 255};
  drawText(renderer, font, color, windowWidth / 2, SNAKE_WIDTH * 2, "Press Any Key to Start!");

  SDL_RenderPresent(renderer);

  bool isReadyToStart = false;
  SDL_Event event;
  while (!isReadyToStart) {
    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
      }
      else if (event.type == SDL_KEYDOWN) {
        isReadyToStart = true;
      }
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, char *msg) {
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, msg, color);
  SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);

  // Draw text such that (x, y) is the center
  SDL_Rect dest = {x - (text_surface->w / 2), y - (text_surface->h / 2), text_surface->w, text_surface->h};
  SDL_RenderCopy(renderer, text, NULL, &dest);

  SDL_DestroyTexture(text);
  SDL_FreeSurface(text_surface);
}

void drawScore(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int windowWidth, int score) {
  // Get string of score
  char scoreText[12] = {0};
  sprintf(scoreText, "Score: %d", score);

  drawText(renderer, font, color, windowWidth / 2, SNAKE_WIDTH, scoreText);
}

void drawGrid(SDL_Renderer *renderer, int windowWidth) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  
  for (int x = 0; x < windowWidth; x += SNAKE_WIDTH) {
    int y = x;
    SDL_RenderDrawLine(renderer, x, 0, x, windowWidth); // Vertical lines
    SDL_RenderDrawLine(renderer, 0, y, windowWidth, y); // Horizontal lines
  }
  
  // Draw Outer Perimeter of grid
  windowWidth--;  // Set to last visible pixel of window
  SDL_RenderDrawLine(renderer, windowWidth, 0, windowWidth, windowWidth);
  SDL_RenderDrawLine(renderer, 0, windowWidth, windowWidth, windowWidth);
}

void endScreen(SDL_Renderer *renderer, TTF_Font *font, int windowWidth, int score) {
  refreshScreen(renderer);

  // Write "Game Over"
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Color color = {255, 255, 255, 255};
  drawText(renderer, font, color, windowWidth / 2, SNAKE_WIDTH * 2, "Game Over!");

  // Write Final Score
  char scoreText[20] = {0};
  sprintf(scoreText, "Final Score: %d", score);
  drawText(renderer, font, color, windowWidth / 2, windowWidth / 2, scoreText);

  SDL_RenderPresent(renderer);

  // Wait for keypress to exit
  bool isReadyToQuit = false;
  SDL_Event event;
  while (!isReadyToQuit) {
    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
        isReadyToQuit = true;
      }
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}
