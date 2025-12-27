#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "gestionSDL.h"

typedef struct player
{
    SDL_Rect position;
    float angle;
    SDL_Texture *texture;
} Player;

Player initPlayer(SDL_Renderer *renderer);
void drawPlayer(SDL_Renderer *renderer, Player player);

#endif