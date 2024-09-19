#include "utils.h"

bool checkCollision(SDL_Rect *r1, SDL_Rect *r2) {
  // Axis Aligned Bounding Box
  // Check if one rectangle is absolutely left of the other
  if ((r1->x + r1->w) <= r2->x || (r2->x + r2->w) <= r1->x) {
    return false;
  }

  // Check if one rectangle is absolutely above the other
  if ((r1->y + r1->h) <= r2->y || (r2->y + r2->h) <= r1->y) {
    return false;
  }

  // Otherwise there is a collision
  return true;
}
