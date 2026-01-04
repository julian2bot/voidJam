#include "items.h"
#include <SDL.h>

SDL_Texture* create_cup_texture(SDL_Renderer *renderer)
{
    const int W = 32, H = 32;
    SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, W, H, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surf) return NULL;

    // Fill transparent background
    SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 0, 0, 0, 0));

    // Colors
    Uint32 brown = SDL_MapRGBA(surf->format, 150, 75, 0, 255);
    Uint32 light = SDL_MapRGBA(surf->format, 200, 120, 50, 255);
    Uint32 white = SDL_MapRGBA(surf->format, 255, 255, 255, 255);

    // Cup body (a rectangle with inner white 'coffee')
    SDL_Rect body = {8, 10, 16, 12};
    SDL_FillRect(surf, &body, brown);
    SDL_Rect coffee = {10, 12, 12, 8};
    SDL_FillRect(surf, &coffee, white);

    // Handle (simple rect)
    SDL_Rect handle = {22, 12, 4, 6};
    SDL_FillRect(surf, &handle, light);

    // Rim highlight
    SDL_Rect rim = {9, 9, 14, 2};
    SDL_FillRect(surf, &rim, light);

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) return NULL;

    // Set blend mode so transparent background works
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    return tex;
}
