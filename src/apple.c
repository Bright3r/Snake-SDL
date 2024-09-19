#include "apple.h"

apple *createApple(int gridRowSize, int snakeWidth) {
  apple *app = (apple *) malloc(sizeof(apple));
  app->w = snakeWidth;
  app->h = snakeWidth;
  moveApple(app, gridRowSize, snakeWidth);

  return app;
}

void destroyApple(apple *app) {
  free(app);
}

void drawApple(SDL_Renderer *renderer, apple *app) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, app);
}

void moveApple(apple *app, int gridRowSize, int snakeWidth) {
  int new_x = (rand() % gridRowSize) * snakeWidth;
  int new_y = (rand() % gridRowSize) * snakeWidth;

  app->x = new_x;
  app->y = new_y;
}
