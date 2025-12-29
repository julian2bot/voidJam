#include "minimap.h"
#include "map.h"
#include <math.h>

#define MM_WIDTH 200
#define MM_HEIGHT 150
#define MM_SCALE 0.25f

int explosion = 1;

void drawMinimap(SDL_Renderer *renderer, Player player, EffectManager *effects)
{
    int viewW, viewH;
    SDL_GetRendererOutputSize(renderer, &viewW, &viewH);

    SDL_Rect mmRect = { 750, viewH - 165, MM_WIDTH, MM_HEIGHT };

    // Background and frame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &mmRect);
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &mmRect);

    SDL_RenderSetClipRect(renderer, &mmRect);

    int cx = mmRect.x + mmRect.w / 2;
    int cy = mmRect.y + mmRect.h / 2;

    float px = player.pos.x;
    float py = player.pos.y;

    int mapW = getMapWidth();
    int mapH = getMapHeight();
    int cellSize = getMapCellSize();

    // Draw cells relative to player
    for (int y = 0; y < mapH; y++) {
        for (int x = 0; x < mapW; x++) {
            int v = getMapCell(x, y);
            if (v <= 0) continue; // skip roads

            int worldX = x * cellSize;
            int worldY = y * cellSize;

            float rx = (worldX - px) * MM_SCALE;
            float ry = (worldY - py) * MM_SCALE;

            SDL_Rect drawR = {
                (int)roundf(cx + rx),
                (int)roundf(cy + ry),
                (int)ceilf(cellSize * MM_SCALE),
                (int)ceilf(cellSize * MM_SCALE)
            };

            if (v == 1) {
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
                SDL_RenderFillRect(renderer, &drawR);
            } else if (v == 2) {
                SDL_SetRenderDrawColor(renderer, 240, 200, 0, 255);
                SDL_Rect t = { drawR.x + drawR.w/4, drawR.y + drawR.h/4, drawR.w/2, drawR.h/2 };
                SDL_RenderFillRect(renderer, &t);
            } else if (v == 3) {
                SDL_SetRenderDrawColor(renderer, 30, 150, 30, 255);
                SDL_RenderFillRect(renderer, &drawR);
            }
        }
    }

    // Player icon at center
    float pw = player.position.w * MM_SCALE;
    float ph = player.position.h * MM_SCALE;
    SDL_Rect pDest = { (int)(cx - pw / 2), (int)(cy - ph / 2), (int)pw, (int)ph };

    if (effects)
    {
        if (explosion && player.tesMort) {
            addExplosion(effects, pDest.x, pDest.y);
            explosion = 0;
        }
    }

    if (player.texture) {
        SDL_RenderCopyEx(renderer, player.texture, NULL, &pDest, player.angle, NULL, SDL_FLIP_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pDest);
    }

    SDL_RenderSetClipRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &mmRect);
}
