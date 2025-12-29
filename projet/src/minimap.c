#include "minimap.h"
#include <math.h>

#define MM_WIDTH 200
#define MM_HEIGHT 150
#define MM_SCALE 0.4f

int explosion = 1;
// ... (previous includes)

void drawMinimap(SDL_Renderer *renderer, Player player, SDL_Rect *walls, int wall_count, EffectManager *effects)
{
    // 1. Définir la zone de la minimap
    int viewW, viewH;
    SDL_GetRendererOutputSize(renderer, &viewW, &viewH);
    
    SDL_Rect mmRect = {
        750, 
        viewH - 165,
        MM_WIDTH, 
        MM_HEIGHT
    };

    // 2. Fond et cadre
    // Fond noir semi-transparent si possible, sinon noir opaque
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &mmRect);
    
    // Cadre gris
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &mmRect);

    // 3. Clipping
    SDL_RenderSetClipRect(renderer, &mmRect);

    int cx = mmRect.x + mmRect.w / 2;
    int cy = mmRect.y + mmRect.h / 2;

    // 4. Murs
    // On utilise une couleur bien visible (ex: Gris clair ou Cyan)
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    float px = player.pos.x;
    float py = player.pos.y;

    for(int i = 0; i < wall_count; i++) {
        SDL_Rect w = walls[i];
        
        // Position relative au joueur
        float rx = (w.x - px) * MM_SCALE;
        float ry = (w.y - py) * MM_SCALE;
        float rw = w.w * MM_SCALE;
        float rh = w.h * MM_SCALE;

        SDL_Rect drawW = {
            (int)(cx + rx),
            (int)(cy + ry),
            (int)ceilf(rw), // Assure au moins 1px si > 0
            (int)ceilf(rh)
        };
        
        // Dessin du mur plein
        SDL_RenderFillRect(renderer, &drawW);
    }

    // 5. Explosions sur la minimap
    

    // 6. Joueur (Texture voiture)
    // Taille scalée
    float pw = player.position.w * MM_SCALE;
    float ph = player.position.h * MM_SCALE;
    
    // Destination rect centrée
    SDL_Rect pDest = {
        (int)(cx - pw / 2),
        (int)(cy - ph / 2),
        (int)pw,
        (int)ph
    };

    if (effects)
    {
        if(explosion && player.tesMort){
            addExplosion(effects, pDest.x, pDest.y);
            explosion = 0;
        }
    }
    // On utilise la texture du joueur avec rotation
    if (player.texture) {
        SDL_RenderCopyEx(renderer, player.texture, NULL, &pDest, player.angle, NULL, SDL_FLIP_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pDest);
    }

    // 7. Fin Clipping
    SDL_RenderSetClipRect(renderer, NULL);
    
    // Petit cadre de finition par dessus
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &mmRect);
}
