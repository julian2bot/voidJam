#include "effects.h"
#include <stdio.h>

EffectManager initEffects(SDL_Renderer *renderer, char* nomAnimation)
{
    EffectManager manager;
    manager.explosionTexture = getTextureFromImage(nomAnimation, renderer);
    
    if (manager.explosionTexture) {
        int w, h;
        SDL_QueryTexture(manager.explosionTexture, NULL, NULL, &w, &h);
        manager.frameWidth = w / EXPLOSION_FRAMES;
 
        manager.frameWidth = w;
        manager.frameHeight = h / EXPLOSION_FRAMES;
    } else {
        manager.frameWidth = 0;
        manager.frameHeight = 0;
    }

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        manager.explosions[i].active = 0;
    }

    return manager;
}

void addExplosion(EffectManager *manager, int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!manager->explosions[i].active)
        {
            manager->explosions[i].active = 1;
            manager->explosions[i].rect.x = x - (manager->frameWidth / 2); // Center it
            manager->explosions[i].rect.y = y - (manager->frameHeight / 2);
            manager->explosions[i].rect.w = manager->frameWidth;
            manager->explosions[i].rect.h = manager->frameHeight;
            manager->explosions[i].currentFrame = 0;
            manager->explosions[i].lastUpdate = SDL_GetTicks();
            break;
        }
    }
}

void updateEffects(EffectManager *manager)
{
    Uint32 currentTime = SDL_GetTicks();
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (manager->explosions[i].active)
        {
            if (currentTime - manager->explosions[i].lastUpdate > EXPLOSION_SPEED)
            {
                manager->explosions[i].currentFrame++;
                manager->explosions[i].lastUpdate = currentTime;
                if (manager->explosions[i].currentFrame >= EXPLOSION_FRAMES)
                {
                    manager->explosions[i].active = 0;
                }
            }
        }
    }
}

void drawEffects(EffectManager *manager, SDL_Renderer *renderer)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (manager->explosions[i].active)
        {
            SDL_Rect srcRect;
            srcRect.x = 0;
            srcRect.y = manager->explosions[i].currentFrame * manager->frameHeight; // Vertical strip
            srcRect.w = manager->frameWidth;
            srcRect.h = manager->frameHeight;

            SDL_RenderCopy(renderer, manager->explosionTexture, &srcRect, &manager->explosions[i].rect);
        }
    }
}

void destroyEffects(EffectManager *manager)
{
    if (manager->explosionTexture)
    {
        SDL_DestroyTexture(manager->explosionTexture);
    }
}
