#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL.h>
#include "player.h"

#include "effects.h"

void drawMinimap(SDL_Renderer *renderer, Player player, SDL_Rect *walls, int wall_count, EffectManager *effects);

#endif
