
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

typedef struct ui_element {
  SDL_Texture *texture;
  SDL_Color color;
  SDL_Rect dest;
} ui_element;

ui_element *createUIElement(SDL_Texture *texture, SDL_Color color, SDL_Rect dest) {
  ui_element *element = (ui_element *) malloc(sizeof(ui_element));
  element->texture = texture;
  element->color = color;
  element->dest = dest;

  return element;
}

ui_element *createUIText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, char **msg) {
  // Draw text such that (x, y) is the center
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, *msg, color);
  SDL_Rect dest = {x - (text_surface->w / 2), y - (text_surface->h / 2), text_surface->w, text_surface->h};

  // Create texture from surface
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  ui_element *textUI = createUIElement(texture, color, dest);
  SDL_FreeSurface(text_surface);
  return textUI;
}

void destroyUIElement(ui_element *element) {
  SDL_DestroyTexture(element->texture);
  free(element);
}

void drawUIElement(SDL_Renderer *renderer, ui_element *element) {
  SDL_SetRenderDrawColor(renderer, element->color.r, element->color.g, element->color.b, element->color.a);
  SDL_RenderCopy(renderer, element->texture, NULL, &element->dest);
}

void drawUI(SDL_Renderer *renderer, ui_element **elements, int numElements) {
  for (int i = 0; i < numElements; i++) {
    drawUIElement(renderer, elements[i]);
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
