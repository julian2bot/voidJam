#ifndef GESTIONSDL_H
#define GESTIONSDL_H
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

SDL_Texture * getTextureFromImage(const char * nomPic, SDL_Renderer * renderer);

#endif