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
