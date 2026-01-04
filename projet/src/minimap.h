#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL.h>
#include "player.h"

#include "effects.h"

void drawMinimap(SDL_Renderer *renderer, Player player, EffectManager *effects);
void reset_minimap_explosion_flag(void);

#endif
