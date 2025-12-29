#include "gestionSDL.h"

SDL_Texture *getTextureFromImage(const char *nomPic, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(nomPic);
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texSprite = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return texSprite;
}

void destroy_textures(unsigned int nb_textures, ...)
{
    va_list args;

    va_start(args, nb_textures);

    for (unsigned int i = 0; i < nb_textures; ++i)
    {
        SDL_DestroyTexture(va_arg(args, SDL_Texture *));
    }

    va_end(args);
}



// draw 
void drawText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color *color, const char *text, int x, int y, int size)
{
    if(text == NULL) return;

    if(font == NULL)
    {
        font = TTF_OpenFont("time.ttf", size);
        if(font == NULL)
        {
            printf("Erreur de chargement de la police : %s", TTF_GetError());
            return;
        }
    }
    SDL_Color c;
    if(color == NULL)
    {
        c.r = 255;
        c.g = 255;
        c.b = 255;
        c.a = 255;
    }
    else
    {
        c = *color; // copie la couleur passée
    }


    SDL_Surface *surface = TTF_RenderText_Solid(font, text, c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void drawRing(SDL_Renderer *r, int cx, int cy, int r1, int r2)
{
    for (int i = 0; i < 360; i++)
    {
        float a = i * M_PI / 180.0f;
        int x1 = cx + cosf(a) * r1;
        int y1 = cy + sinf(a) * r1;
        int x2 = cx + cosf(a) * r2;
        int y2 = cy + sinf(a) * r2;
        SDL_RenderDrawLine(r, x1, y1, x2, y2);
    }
}
 void drawPrevIcon(SDL_Renderer *renderer, SDL_Rect r)
{
    int cx = r.x + r.w / 2;
    int cy = r.y + r.h / 2;
    int size = r.w / 3;

    SDL_RenderDrawLine(renderer,
        cx + size, cy - size,
        cx - size, cy);

    SDL_RenderDrawLine(renderer,
        cx - size, cy,
        cx + size, cy + size);
}

 void drawNextIcon(SDL_Renderer *renderer, SDL_Rect r)
{
    int cx = r.x + r.w / 2;
    int cy = r.y + r.h / 2;
    int size = r.w / 3;

    SDL_RenderDrawLine(renderer,
        cx - size, cy - size,
        cx + size, cy);

    SDL_RenderDrawLine(renderer,
        cx + size, cy,
        cx - size, cy + size);
}

 void drawPauseIcon(SDL_Renderer *renderer, SDL_Rect r)
{
    int barWidth  = r.w / 6;
    int barHeight = r.h / 2;

    SDL_Rect leftBar = {
        r.x + r.w / 2 - barWidth - 3,
        r.y + r.h / 2 - barHeight / 2,
        barWidth,
        barHeight
    };

    SDL_Rect rightBar = {
        r.x + r.w / 2 + 3,
        r.y + r.h / 2 - barHeight / 2,
        barWidth,
        barHeight
    };

    SDL_RenderFillRect(renderer, &leftBar);
    SDL_RenderFillRect(renderer, &rightBar);
}