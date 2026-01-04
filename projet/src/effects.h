#ifndef EFFECTS_H
#define EFFECTS_H

#include <SDL.h>
#include <SDL_image.h>
#include "gestionSDL.h"

#define MAX_EXPLOSIONS 20
#define EXPLOSION_FRAMES 5
#define EXPLOSION_SPEED 140 // ms per frame

typedef struct {
    int active;
    SDL_Rect rect; // Position on screen
    int currentFrame;
    Uint32 lastUpdate;
} Explosion;

typedef struct {
    Explosion explosions[MAX_EXPLOSIONS];
    SDL_Texture *explosionTexture;
    int frameWidth;
    int frameHeight;
} EffectManager;

EffectManager initEffects(SDL_Renderer *renderer, char* nomAnimation);
void addExplosion(EffectManager *manager, int x, int y);
void updateEffects(EffectManager *manager);
void drawEffects(EffectManager *manager, SDL_Renderer *renderer);
void destroyEffects(EffectManager *manager);

#endif
