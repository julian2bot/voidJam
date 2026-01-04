#ifndef ITEMS_H
#define ITEMS_H

#include <SDL.h>

/* Create a 32x32 cup texture (fallback if cup.png missing) */
SDL_Texture* create_cup_texture(SDL_Renderer *renderer);

#endif
