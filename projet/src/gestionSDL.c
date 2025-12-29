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
