#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

static SDL_Color COLOR_BLACK = {0, 0, 0, 255};
static SDL_Color COLOR_WHITE = {255, 255, 255, 255};
static SDL_Color COLOR_BG = {148, 148, 184, 255};

typedef struct ui_element {
  SDL_Texture *texture;
  SDL_Color color;
  SDL_Rect dest;
} ui_element;

ui_element *createUIElement(SDL_Texture *texture, SDL_Color color, SDL_Rect dest);
ui_element *createUIText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, char **msg);
void destroyUIElement(ui_element *element);
void refreshScreen(SDL_Renderer *renderer);
void drawUIElement(SDL_Renderer *renderer, ui_element *element);
void drawUI(SDL_Renderer *renderer, ui_element **elements, int numElements);
void drawText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, char *msg);

#endif
